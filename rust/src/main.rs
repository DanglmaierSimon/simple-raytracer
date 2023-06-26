#![warn(clippy::correctness)]
#![warn(clippy::suspicious)]
#![warn(clippy::complexity)]
#![warn(clippy::style)]
#![warn(clippy::perf)]

pub mod camera;
pub mod dielectric;
pub mod hittable;
pub mod lambertian;
pub mod material;
pub mod metal;
pub mod ray;
pub mod sphere;
pub mod utils;
pub mod vec3;

use std::{
    f64::INFINITY,
    io::Write,
    sync::{mpsc::channel, Arc},
};

use hittable::{HitRecord, HitResult, Hittable};
use material::Material;
use rand::{rngs::ThreadRng, thread_rng, Rng};
use ray::Ray;
use threadpool::ThreadPool;
use vec3::Vec3;

use crate::{
    camera::Camera, dielectric::Dielectric, hittable::HittableList, lambertian::Lambertian,
    metal::Metal, sphere::Sphere, utils::write_color, vec3::Color,
};

#[derive(Debug, Clone)]
struct ImageData {
    pub samples_per_pixel: usize,
    pub image_width: usize,
    pub image_height: usize,
}

fn random_scene() -> Arc<HittableList> {
    let mut world = HittableList::default();

    let ground_material = Arc::new(Lambertian::new(Vec3(0.5, 0.5, 0.5)));
    world.add(Arc::new(Sphere::new(
        Vec3(0.0, -1000.0, 0.0),
        1000.0,
        ground_material,
    )));

    for a in -11..11 {
        for b in -11..11 {
            let choose_mat: f64 = thread_rng().gen();

            let center = Vec3(
                (a as f64) + 0.9 * thread_rng().gen::<f64>(),
                0.2,
                (b as f64) + 0.9 * thread_rng().gen::<f64>(),
            );

            if (center - Vec3(4.0, 0.2, 0.0)).length() > 0.9 {
                let sphere_material: Arc<dyn Material>;

                if choose_mat < 0.8 {
                    // diffuse
                    let albedo = Color::random_vec() * Color::random_vec();
                    sphere_material = Arc::new(Lambertian::new(albedo));
                    world.add(Arc::new(Sphere::new(center, 0.2, sphere_material)));
                } else if choose_mat < 0.95 {
                    // metal
                    let albedo = Color::random(0.5, 1.0);
                    let fuzz = thread_rng().gen_range(0.0..0.5);
                    sphere_material = Arc::new(Metal::new(albedo, fuzz));
                    world.add(Arc::new(Sphere::new(center, 0.2, sphere_material)));
                } else {
                    // glass
                    sphere_material = Arc::new(Dielectric::new(1.5));
                    world.add(Arc::new(Sphere::new(center, 0.2, sphere_material)));
                }
            }
        }
    }

    {
        let material = Arc::new(Dielectric::new(1.5));
        world.add(Arc::new(Sphere::new(Vec3(0.0, 1.0, 0.0), 1.0, material)));
    }

    {
        let material = Arc::new(Lambertian::new(Vec3(0.4, 0.2, 0.1)));
        world.add(Arc::new(Sphere::new(Vec3(-4.0, 1.0, 0.0), 1.0, material)));
    }

    {
        let material = Arc::new(Metal::new(Vec3(0.7, 0.6, 0.5), 0.0));
        world.add(Arc::new(Sphere::new(Vec3(4.0, 1.0, 0.0), 1.0, material)));
    }

    Arc::new(world)
}

fn ray_color(r: Ray, world: Arc<dyn Hittable>, depth: u32) -> Color {
    let mut rec = HitRecord::default();

    // if we exceed the ray bounce limit, no more light is gathered
    if depth == 0 {
        return Vec3(0.0, 0.0, 0.0);
    }

    if let HitResult::Hit(mat) = world.hit(r, 0.001, INFINITY, &mut rec) {
        let mut scattered = Ray::default();
        let mut attenuation = Color::default();

        if mat.scatter(r, &rec, &mut attenuation, &mut scattered) {
            return attenuation * ray_color(scattered, world, depth - 1);
        } else {
            return Vec3(0.0, 0.0, 0.0);
        }
    }

    let unit_dir = Vec3::unit_vector(r.direction());
    let t = 0.5 * (unit_dir.y() + 1.0);
    (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0)
}

struct ImageLine {
    pub idx: usize,       // y-coordinate
    pub line: Vec<Color>, // image width
}

fn calculate_single_image_line(
    line_idx: usize,
    data: &ImageData,
    world: Arc<dyn Hittable>,
    max_depth: u32,
    cam: &Camera,
) -> ImageLine {
    let mut rng = thread_rng();
    let mut res = Vec::with_capacity(data.image_width);

    for i in 0..data.image_width {
        res.insert(
            i,
            calculate_single_pixel(i, line_idx, data, world.clone(), max_depth, cam, &mut rng),
        );
    }

    ImageLine {
        idx: line_idx,
        line: res,
    }
}

fn calculate_single_pixel(
    i: usize,
    j: usize,
    data: &ImageData,
    world: Arc<dyn Hittable>,
    max_depth: u32,
    cam: &Camera,
    rng: &mut ThreadRng,
) -> Color {
    let mut pixel_color = Vec3(0.0, 0.0, 0.0);
    for _ in 0..data.samples_per_pixel {
        let r1: f64 = rng.gen();
        let r2: f64 = rng.gen();

        let u = (i as f64 + r1) / (data.image_width - 1) as f64;
        let v = (j as f64 + r2) / (data.image_height - 1) as f64;

        let r = cam.get_ray(u, v);
        pixel_color += ray_color(r, world.clone(), max_depth);
    }

    pixel_color
}

enum ThreadStatus {
    Started,
    Finished,
}

fn calculate_all_pixels(
    data: &ImageData,
    world: Arc<dyn Hittable>,
    cam: &Camera,
) -> Vec<ImageLine> {
    let max_depth: u32 = 100;

    let n_workers = 16;

    let (status_tx, status_rx) = channel::<ThreadStatus>();
    let (tx, rx) = channel::<ImageLine>();

    let pool = ThreadPool::new(n_workers);
    eprintln!("creating jobs for pool");
    for j in 0..data.image_height - 1 {
        let world = world.clone();
        let cam = cam.clone();
        let tx = tx.clone();
        let status_tx = status_tx.clone();
        let data = data.clone();
        status_tx.send(ThreadStatus::Started).unwrap();

        pool.execute(move || {
            let il: ImageLine = calculate_single_image_line(j, &data, world, max_depth, &cam);

            tx.send(il).unwrap();
            status_tx.send(ThreadStatus::Finished).unwrap();
        });
    }
    eprintln!("done creating jobs");

    // drop to not have a deadlock
    drop(tx);
    drop(status_tx);

    let mut remaining_lines = 0;

    status_rx.iter().for_each(|v| {
        match v {
            ThreadStatus::Started => remaining_lines += 1,
            ThreadStatus::Finished => remaining_lines -= 1,
        };
        eprint!("\rLines remaining: {:0>5}", remaining_lines);
        std::io::stderr().flush().unwrap();
    });

    return rx.iter().collect();
}

fn write_colors(pixels: Vec<Color>, data: &ImageData) {
    println!("P3\n{} {}\n255\n", data.image_width, data.image_height);

    for pixel_color in pixels {
        write_color(&mut std::io::stdout(), pixel_color, data.samples_per_pixel).unwrap();
    }
}

fn main() {
    // image data
    let aspect_ratio = 16.0 / 9.0;
    let img_width: usize = 1200;

    let image_data = ImageData {
        samples_per_pixel: 500,
        image_width: img_width,
        image_height: (img_width as f64 / aspect_ratio) as usize,
    };

    // world
    let world = random_scene();

    // camera
    let lookfrom = Vec3(13.0, 2.0, 3.0);
    let lookat = Vec3(0.0, 0.0, 0.0);
    let vup = Vec3(0.0, 1.0, 0.0);
    let dist_to_focus = 10.0;
    let aperture = 0.1;

    let cam = Camera::new(
        lookfrom,
        lookat,
        vup,
        20.0,
        aspect_ratio,
        aperture,
        dist_to_focus,
    );

    // render
    let mut image_lines = calculate_all_pixels(&image_data, world, &cam);

    image_lines.sort_by(|lhs, rhs| lhs.idx.cmp(&rhs.idx));
    image_lines.reverse();

    let r: Vec<Vec<Color>> = image_lines.iter().map(|elem| elem.line.clone()).collect();

    let final_r = r.into_iter().flatten().collect();

    write_colors(final_r, &image_data);

    eprintln!("\nDone!");
}
