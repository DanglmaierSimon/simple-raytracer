use std::{f64::INFINITY, io::Write, sync::mpsc::channel};

use rand::{rngs::ThreadRng, thread_rng, Rng};
use threadpool::ThreadPool;
use threadpool_scope::scope_with;

use crate::{
    camera::Camera,
    hittable::{HitRecord, HitResult, Hittable, HittableList},
    ppm_image::PpmImage,
    ray::Ray,
    utils::clamp,
    vec3::{Color, Vec3},
};

pub struct Renderer {
    world: HittableList,
    target: PpmImage,
}

impl Renderer {
    pub fn new(world: HittableList, target: PpmImage) -> Self {
        Self { world, target }
    }

    pub fn render(
        mut self,
        samples_per_pixel: usize,
        max_depth: usize,
        aspect_ratio: f64,
    ) -> PpmImage {
        // camera
        // TODO: Make camaera part of the world
        // TODO: make world into struct with Cam, cam position and list of objects inside
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
            0.0,
            1.0,
        );

        // render
        let mut image_lines = self.calculate_all_pixels(max_depth, &cam);

        image_lines.sort_by(|lhs, rhs| lhs.0.cmp(&rhs.0));
        image_lines.reverse();

        let r: Vec<Vec<Color>> = image_lines.iter().map(|elem| elem.1.clone()).collect();

        let final_r = r.into_iter().flatten().collect();

        self.write_colors(samples_per_pixel, final_r);

        self.target
    }

    fn calculate_single_pixel(
        &self,
        i: usize,
        j: usize,
        max_depth: usize,
        samples_per_pixel: usize,
        cam: &Camera,
        rng: &mut ThreadRng,
    ) -> Color {
        let mut pixel_color = Vec3(0.0, 0.0, 0.0);
        for _ in 0..samples_per_pixel {
            let r1: f64 = rng.gen();
            let r2: f64 = rng.gen();

            let u = (i as f64 + r1) / (self.target.width() - 1) as f64;
            let v = (j as f64 + r2) / (self.target.height() - 1) as f64;

            let r = cam.get_ray(u, v);
            pixel_color += self.ray_color(r, max_depth);
        }

        pixel_color
    }

    fn ray_color(&self, r: Ray, depth: usize) -> Color {
        let mut rec = HitRecord::default();

        // if we exceed the ray bounce limit, no more light is gathered
        if depth == 0 {
            return Vec3(0.0, 0.0, 0.0);
        }

        if let HitResult::Hit(mat) = self.world.hit(r, 0.001, INFINITY, &mut rec) {
            let mut scattered = Ray::default();
            let mut attenuation = Color::default();

            if mat.scatter(r, &rec, &mut attenuation, &mut scattered) {
                return attenuation * self.ray_color(scattered, depth - 1);
            } else {
                return Vec3(0.0, 0.0, 0.0);
            }
        }

        let unit_dir = Vec3::unit_vector(r.direction());
        let t = 0.5 * (unit_dir.y() + 1.0);
        (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0)
    }

    fn calculate_single_image_line(
        &self,
        line_idx: usize,
        max_depth: usize,
        cam: &Camera,
    ) -> (usize, Vec<Color>) {
        let mut rng = thread_rng();
        let mut res = Vec::with_capacity(self.target.width());

        for i in 0..self.target.width() {
            res.insert(
                i,
                self.calculate_single_pixel(i, line_idx, max_depth, max_depth, cam, &mut rng),
            );
        }

        (line_idx, res)
    }

    fn calculate_all_pixels(&self, max_depth: usize, cam: &Camera) -> Vec<(usize, Vec<Color>)> {
        enum ThreadStatus {
            Queued,
            Started,
            Finished,
        }

        let n_workers = 16;

        let (status_tx, status_rx) = channel::<ThreadStatus>();
        let (tx, rx) = channel::<(usize, Vec<Color>)>();

        let pool = ThreadPool::new(n_workers);
        println!("creating jobs for pool");

        scope_with(&pool, |scope| {
            for j in 0..self.target.height() {
                let tx = tx.clone();
                let status_tx = status_tx.clone();
                status_tx.send(ThreadStatus::Queued).unwrap();

                scope.execute(move || {
                    status_tx.send(ThreadStatus::Started).unwrap();
                    let il = self.calculate_single_image_line(j, max_depth, cam);

                    tx.send(il).unwrap();
                    status_tx.send(ThreadStatus::Finished).unwrap();
                });
            }
            println!("done creating jobs");

            // drop to not have a deadlock
            drop(tx);
            drop(status_tx);

            let mut remaining_jobs = 0;
            let mut running_jobs = 0;
            let mut finished_jobs = 0;

            status_rx.iter().for_each(|v| {
                match v {
                    ThreadStatus::Queued => remaining_jobs += 1,
                    ThreadStatus::Started => {
                        remaining_jobs -= 1;
                        running_jobs += 1
                    }
                    ThreadStatus::Finished => {
                        running_jobs -= 1;
                        finished_jobs += 1
                    }
                };
                print!(
                    "\rJobs remaining/running/finished: {:0>5}/{:0>5}/{:0>5}",
                    remaining_jobs, running_jobs, finished_jobs
                );
                std::io::stdout().flush().unwrap();
            });
        });

        pool.join();

        return rx.iter().collect();
    }

    fn write_colors(&mut self, samples_per_pixel: usize, pixels: Vec<Color>) {
        for (idx, pixel_color) in pixels.into_iter().enumerate() {
            self.write_color(pixel_color, samples_per_pixel, idx);
        }
    }

    pub fn write_color(&mut self, pixel_color: Color, samples_per_pixel: usize, idx: usize) {
        let (mut r, mut g, mut b) = (pixel_color.x(), pixel_color.y(), pixel_color.z());

        // divide the color by the number of samples and gamma correct for gamma=2.0
        let scale = 1.0 / samples_per_pixel as f64;
        r = (scale * r).sqrt();
        g = (scale * g).sqrt();
        b = (scale * b).sqrt();

        self.target.set_value(
            idx,
            (
                (256.0 * clamp(r, 0.0, 0.999)) as u8,
                (256.0 * clamp(g, 0.0, 0.999)) as u8,
                (256.0 * clamp(b, 0.0, 0.999)) as u8,
            ),
        );
    }
}
