use crate::{
    hittable::HitRecord,
    material::Material,
    ray::Ray,
    vec3::{Color, Vec3},
};

pub struct Metal {
    albedo: Color,
    fuzz: f64,
}

impl Metal {
    pub fn new(albedo: Color, f: f64) -> Self {
        Self {
            albedo,
            fuzz: if f < 1.0 { f } else { 1.0 },
        }
    }
}

impl Material for Metal {
    fn scatter(
        &self,
        r_in: Ray,
        rec: &HitRecord,
        attenuation: &mut Color,
        scattered: &mut Ray,
    ) -> bool {
        let reflected = Vec3::reflect(Vec3::unit_vector(r_in.direction()), rec.normal);
        *scattered = Ray::new(
            rec.p,
            reflected + self.fuzz * Vec3::random_in_unit_sphere(),
            r_in.time(),
        );
        *attenuation = self.albedo;
        Vec3::dot(scattered.direction(), rec.normal) > 0.0
    }
}
