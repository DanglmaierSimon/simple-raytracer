#![deny(clippy::all)]
#![warn(clippy::pedantic)]
#![warn(clippy::restriction)]
#![warn(clippy::nursery)]
#![warn(clippy::cargo)]

pub mod camera;
pub mod hittable;
pub mod ray;
pub mod sphere;
pub mod utils;
pub mod vec3;

use std::{
    f64::INFINITY,
    io::{self, Write},
    rc::Rc,
};

use hittable::{HitRecord, Hittable};
use rand::Rng;
use ray::Ray;
use vec3::Vec3;

use crate::{
    camera::Camera,
    hittable::HittableList,
    sphere::Sphere,
    utils::write_color,
    vec3::{Color, Point3},
};

fn ray_color(r: Ray, world: &impl Hittable, depth: u64) -> Color {
    let mut rec = HitRecord::default();

    // if we exceed the ray bounce limit, no more light is gathered
    if depth <= 0 {
        return Color::new(0.0, 0.0, 0.0);
    }

    if world.hit(r, 0.001, INFINITY, &mut rec) {
        let target = rec.p + Vec3::random_in_hemisphere(rec.normal);
        return 0.5 * ray_color(Ray::new(rec.p, target - rec.p), world, depth - 1);
    }

    let unit_dir = Vec3::unit_vector(r.direction());
    let t = 0.5 * (unit_dir.y() + 1.0);
    (1.0 - t) * Color::new(1.0, 1.0, 1.0) + t * Color::new(0.5, 0.7, 1.0)
}

fn main() {
    // image data
    let aspect_ratio = 16.0 / 9.0;
    let img_width = 400;
    let img_height = (img_width as f64 / aspect_ratio) as i32;
    let samples_per_pixel = 100;
    let max_depth = 50;

    // camera
    let cam = Camera::new();

    // world
    let world = HittableList::new(vec![
        Rc::new(Sphere::new(Point3::new(0.0, 0.0, -1.0), 0.5)),
        Rc::new(Sphere::new(Point3::new(0.0, -100.5, -1.0), 100.0)),
    ]);

    // render
    let mut rng = rand::thread_rng();
    println!("P3\n{} {}\n255\n", img_width, img_height);

    for j in (0..img_height - 1).rev() {
        eprint!("\rScanlines remaining: {:0>5}", j);
        io::stderr().flush().unwrap();
        for i in 0..img_width {
            let mut pixel_color = Color::new(0.0, 0.0, 0.0);
            for _ in 0..samples_per_pixel {
                let r1: f64 = rng.gen();
                let r2: f64 = rng.gen();

                let u = (i as f64 + r1) / (img_width - 1) as f64;
                let v = (j as f64 + r2) / (img_height - 1) as f64;

                let r = cam.get_ray(u, v);
                pixel_color += ray_color(r, &world, max_depth);
            }
            write_color(&mut std::io::stdout(), pixel_color, samples_per_pixel).unwrap();
        }
    }

    eprintln!("\nDone!");
}
