use crate::{
    ray::Ray,
    utils::random_in_range,
    vec3::{Point3, Vec3},
};

#[derive(Debug, Clone)]
pub struct Camera {
    origin: Point3,
    lower_left_corner: Point3,
    horizontal: Vec3,
    vertical: Vec3,
    u: Vec3,
    v: Vec3,
    // w: Vec3,
    lens_radius: f64,
    time_0: f64,
    time_1: f64,
}

impl Camera {
    pub fn new(
        lookfrom: Point3,
        lookat: Point3,
        vup: Vec3,
        vfov: f64, // vertical field-of-view in degrees
        aspect_ratio: f64,
        aperture: f64,
        focus_dist: f64,
        time_0: f64, // shutter open time
        time_1: f64, // shutter close time
    ) -> Self {
        let theta = vfov.to_radians();
        let h = (theta / 2.0).tan();
        let viewport_height = 2.0 * h;
        let viewport_width = aspect_ratio * viewport_height;

        let w = Vec3::unit_vector(lookfrom - lookat);
        let u = Vec3::unit_vector(Vec3::cross(vup, w));
        let v = Vec3::cross(w, u);

        let origin = lookfrom;
        let horizontal = focus_dist * viewport_width * u;
        let vertical = focus_dist * viewport_height * v;
        let lower_left_corner = origin - (horizontal / 2.0) - (vertical / 2.0) - (focus_dist * w);
        let lens_radius = aperture / 2.0;

        Self {
            origin,
            horizontal,
            vertical,
            lower_left_corner,
            // w,
            v,
            u,
            lens_radius,
            time_0,
            time_1,
        }
    }

    pub fn get_ray(&self, s: f64, t: f64) -> Ray {
        let rd = self.lens_radius * Vec3::random_in_unit_disk();
        let offset = self.u * rd.x() + self.v * rd.y();

        Ray::new(
            self.origin + offset,
            self.lower_left_corner + s * self.horizontal + t * self.vertical - self.origin - offset,
            random_in_range(self.time_0, self.time_1),
        )
    }
}
