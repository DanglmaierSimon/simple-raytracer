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
pub mod ppm_image;
pub mod ray;
pub mod renderer;
pub mod sphere;
pub mod utils;
pub mod vec3;

use std::sync::Arc;

use material::Material;

use rand::{thread_rng, Rng};
use vec3::Vec3;

use crate::{
    dielectric::Dielectric, hittable::HittableList, lambertian::Lambertian, metal::Metal,
    ppm_image::PpmImage, renderer::Renderer, sphere::Sphere, vec3::Color,
};

fn random_scene() -> HittableList {
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

    world
}

fn main() {
    // image data
    let aspect_ratio = 3.0 / 2.0;
    let image_width: usize = 1200;

    let image_height = (image_width as f64 / aspect_ratio) as usize;

    let image = PpmImage::new(image_width, image_height, ppm_image::PpmImageType::PlainPpm);

    // world
    let world = random_scene();
    let renderer = Renderer::new(world, image);

    let result = renderer.render(100, 50, aspect_ratio);

    // TODO: Pass image name via command line
    let output_file = std::fs::File::create("image.ppm").unwrap();
    result.write_to_file(output_file).unwrap();

    println!("\nDone!");
}
