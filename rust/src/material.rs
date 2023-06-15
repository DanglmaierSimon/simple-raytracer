use crate::{hittable::HitRecord, ray::Ray, vec3::Color};

pub trait Material: Send + Sync {
    fn scatter(
        &self,
        r_in: Ray,
        rec: &HitRecord,
        attenuation: &mut Color,
        scattered: &mut Ray,
    ) -> bool;
}
