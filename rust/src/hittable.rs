use std::sync::Arc;

use crate::{
    material::Material,
    ray::Ray,
    vec3::{Point3, Vec3},
};

#[derive(Clone, Default)]
pub struct HitRecord {
    pub p: Point3,
    pub normal: Vec3,
    pub t: f64,

    front_face: bool,
}

impl HitRecord {
    pub fn set_front_face(&mut self, r: Ray, outward_normal: Vec3) {
        self.front_face = Vec3::dot(r.direction(), outward_normal) < 0.0;
        self.normal = if self.front_face() {
            outward_normal
        } else {
            -outward_normal
        };
    }

    pub fn front_face(&self) -> bool {
        self.front_face
    }
}

pub enum HitResult {
    Miss,
    Hit(Arc<dyn Material>),
}

impl HitResult {
    /// Returns `true` if the hit result is [`Miss`].
    ///
    /// [`Miss`]: HitResult::Miss
    #[must_use]
    pub fn is_miss(&self) -> bool {
        matches!(self, Self::Miss)
    }

    /// Returns `true` if the hit result is [`Hit`].
    ///
    /// [`Hit`]: HitResult::Hit
    #[must_use]
    pub fn is_hit(&self) -> bool {
        matches!(self, Self::Hit(..))
    }

    pub fn as_hit(&self) -> Option<&Arc<dyn Material>> {
        if let Self::Hit(v) = self {
            Some(v)
        } else {
            None
        }
    }
}

pub trait Hittable: Send + Sync {
    fn hit(&self, r: Ray, t_min: f64, t_max: f64, rec: &mut HitRecord) -> HitResult;
}

#[derive(Default)]
pub struct HittableList {
    objects: Vec<Arc<dyn Hittable>>,
}

impl HittableList {
    pub fn new(objects: Vec<Arc<dyn Hittable>>) -> Self {
        Self { objects }
    }

    pub fn clear(&mut self) {
        self.objects.clear();
    }

    pub fn add(&mut self, object: Arc<dyn Hittable>) {
        self.objects.push(object)
    }
}

impl Hittable for HittableList {
    fn hit(&self, r: Ray, t_min: f64, t_max: f64, rec: &mut HitRecord) -> HitResult {
        let mut temp_rec = HitRecord::default();
        let mut hit_anything = false;
        let mut closest_so_far = t_max;
        let mut last_hit_material: Option<Arc<dyn Material>>;
        last_hit_material = None;

        for obj in &self.objects {
            if let HitResult::Hit(mat) = obj.hit(r, t_min, closest_so_far, &mut temp_rec) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                *rec = temp_rec.clone();
                last_hit_material = Some(mat);
            }
        }

        if hit_anything {
            assert!(last_hit_material.is_some());
            HitResult::Hit(last_hit_material.unwrap())
        } else {
            assert!(last_hit_material.is_none());
            HitResult::Miss
        }
    }
}
