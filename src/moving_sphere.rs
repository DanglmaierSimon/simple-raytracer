use std::sync::Arc;

use crate::{
    hittable::{HitResult, Hittable},
    material::Material,
    vec3::{Point3, Vec3},
};

pub struct MovingSphere {
    center_0: Point3,
    center_1: Point3,
    time_0: f64,
    time_1: f64,
    radius: f64,
    mat: Arc<dyn Material>,
}

impl MovingSphere {
    pub fn new(
        center_0: Point3,
        center_1: Point3,
        time_0: f64,
        time_1: f64,
        radius: f64,
        mat: Arc<dyn Material>,
    ) -> Self {
        Self {
            center_0,
            center_1,
            time_0,
            time_1,
            radius,
            mat,
        }
    }

    pub fn center(&self, time: f64) -> Point3 {
        self.center_0
            + ((time - self.time_0) / (self.time_1 - self.time_0)) * (self.center_1 - self.center_0)
    }

    pub fn radius(&self) -> f64 {
        self.radius
    }
}

impl Hittable for MovingSphere {
    fn hit(
        &self,
        r: crate::ray::Ray,
        t_min: f64,
        t_max: f64,
        rec: &mut crate::hittable::HitRecord,
    ) -> crate::hittable::HitResult {
        let oc = r.origin() - self.center(r.time());
        let a = r.direction().length_squared();
        let half_b = Vec3::dot(oc, r.direction());
        let c = oc.length_squared() - self.radius() * self.radius();

        let discriminant = half_b * half_b - a * c;
        if discriminant < 0.0 {
            return HitResult::Miss;
        }

        let sqrtd = discriminant.sqrt();

        // find nearest root that lies in the acceptable range
        let mut root = (-half_b - sqrtd) / a;
        if root < t_min || t_max < root {
            root = (-half_b + sqrtd) / a;
            if root < t_min || t_max < root {
                return HitResult::Miss;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        rec.normal = (rec.p - self.center(r.time())) / self.radius();
        let outward_normal = (rec.p - self.center(r.time())) / self.radius();
        rec.set_front_face(r, outward_normal);

        HitResult::Hit(self.mat.clone())
    }
}
