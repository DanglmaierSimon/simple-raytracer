#![deny(clippy::all)]
#![warn(clippy::pedantic)]
#![warn(clippy::restriction)]
#![warn(clippy::nursery)]
#![warn(clippy::cargo)]

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
    io::{self, Write},
    sync::Arc,
    thread::scope,
};

use hittable::{HitRecord, Hittable};
use material::Material;
use rand::{thread_rng, Rng};
use ray::Ray;
use vec3::Vec3;

use crate::{
    camera::Camera, dielectric::Dielectric, hittable::HittableList, lambertian::Lambertian,
    metal::Metal, sphere::Sphere, utils::write_color, vec3::Color,
};

const samples_per_pixel: usize = 500;

fn random_scene() -> Arc<HittableList> {
    let mut world = HittableList::default();

    let ground_material = Arc::new(Lambertian::new(Vec3(0.5, 0.5, 0.5)));
    world.add(Arc::new(Sphere::new(
        Vec3(0.0, -1000.0, 0.0),
        1000.0,
        ground_material.clone(),
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

    return Arc::new(world);
}

fn ray_color(r: Ray, world: Arc<dyn Hittable>, depth: u32) -> Color {
    let mut rec = HitRecord::default();

    // if we exceed the ray bounce limit, no more light is gathered
    if depth <= 0 {
        return Vec3(0.0, 0.0, 0.0);
    }

    if world.hit(r, 0.001, INFINITY, &mut rec) {
        let mut scattered = Ray::default();
        let mut attenuation = Color::default();

        let material = rec.mat.clone().unwrap();

        if material.scatter(r, &rec, &mut attenuation, &mut scattered) {
            return attenuation * ray_color(scattered, world, depth - 1);
        } else {
            return Vec3(0.0, 0.0, 0.0);
        }
    }

    let unit_dir = Vec3::unit_vector(r.direction());
    let t = 0.5 * (unit_dir.y() + 1.0);
    (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0)
}

// TODO: parallelize with threadpools
// TODO: Return a struct of pixel idx & pixel color here!
fn calculate_single_pixel(
    i: usize,
    j: usize,
    img_width: usize,
    img_height: usize,
    world: Arc<dyn Hittable>,
    max_depth: u32,
    cam: &Camera,
    counter: usize,
) -> result {
    let mut pixel_color = Vec3(0.0, 0.0, 0.0);
    let mut rng = thread_rng();
    for _ in 0..samples_per_pixel {
        let r1: f64 = rng.gen();
        let r2: f64 = rng.gen();

        let u = (i as f64 + r1) / (img_width - 1) as f64;
        let v = (j as f64 + r2) / (img_height - 1) as f64;

        let r = cam.get_ray(u, v);
        pixel_color += ray_color(r, world.clone(), max_depth);
    }
    return result(counter, pixel_color);
}

struct result(pub usize, pub Color);

fn calculate_all_pixels(
    img_width: usize,
    img_height: usize,
    world: Arc<dyn Hittable>,
    cam: Camera,
) -> Vec<result> {
    let mut counter: usize = 0;

    let max_depth = 100;

    let mut res = Vec::new();

    scope(|s| {
        let mut threads = Vec::new();

        for j in (0..img_height - 1).rev() {
            eprint!("\rScanlines remaining: {:0>5}", j);
            io::stderr().flush().unwrap();
            for i in 0..img_width {
                counter += 1;

                let world = world.clone();
                let cam = cam.clone();

                let t = s.spawn(move || {
                    let c = cam.clone();
                    calculate_single_pixel(
                        i, j, img_width, img_height, world, max_depth, &c, counter,
                    )
                });
                threads.push(t);
            }
        }

        for tres in threads {
            res.push(tres.join().unwrap());
        }
    });

    return res;
}

fn write_colors(pixels: Vec<Color>, img_width: usize, img_height: usize) {
    println!("P3\n{} {}\n255\n", img_width, img_height);

    for pixel_color in pixels {
        write_color(&mut std::io::stdout(), pixel_color, samples_per_pixel).unwrap();
    }
}

fn main() {
    // image da
    let aspect_ratio = 3.0 / 2.0;
    let img_width: usize = 1200;
    let img_height: usize = (img_width as f64 / aspect_ratio) as usize;

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
    let mut pixels = calculate_all_pixels(img_width, img_height, world, cam);

    pixels.sort_by(|lhs, rhs| lhs.0.cmp(&rhs.0));

    let r: Vec<Color> = pixels.iter().map(|elem| elem.1).collect();

    write_colors(r, img_width, img_height);

    eprintln!("\nDone!");
}
