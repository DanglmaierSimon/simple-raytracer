use rand::thread_rng;

use crate::{
    hittable::HitRecord,
    material::Material,
    ray::Ray,
    vec3::{Color, Vec3},
};

#[derive(Debug, Clone, Copy)]
pub struct Dielectric {
    ir: f64, // index of refraction
}

impl Dielectric {
    pub fn new(ir: f64) -> Self {
        Self { ir }
    }
}

impl Material for Dielectric {
    fn scatter(
        &self,
        r_in: Ray,
        rec: &HitRecord,
        attenuation: &mut Color,
        scattered: &mut Ray,
    ) -> bool {
        *attenuation = Vec3(1.0, 1.0, 1.0);
        let refraction_ratio = if rec.front_face() {
            1.0 / self.ir
        } else {
            self.ir
        };

        let unit_direction = Vec3::unit_vector(r_in.direction());

        let cos_theta = f64::min(Vec3::dot(-unit_direction, rec.normal), 1.0);
        let sin_theta = (1.0 - cos_theta * cos_theta).sqrt();

        let cannot_refract = refraction_ratio * sin_theta > 1.0;

        let mut rng = thread_rng();

        let direction = if cannot_refract
            || reflectance(cos_theta, refraction_ratio) > rand::Rng::gen(&mut rng)
        {
            Vec3::reflect(unit_direction, rec.normal)
        } else {
            Vec3::refract(unit_direction, rec.normal, refraction_ratio)
        };

        *scattered = Ray::new(rec.p, direction);

        return true;
    }
}

fn reflectance(cosine: f64, ref_idx: f64) -> f64 {
    // use schlick's approximation for reflectance
    let mut r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * (1.0 - cosine).powi(5);
}
