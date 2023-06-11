use std::{fmt::Display, ops};

use rand::Rng;

#[derive(Debug, Copy, Clone)]
pub struct Vec3 {
    e: [f64; 3],
}

impl Vec3 {
    pub fn new(e1: f64, e2: f64, e3: f64) -> Self {
        Self { e: [e1, e2, e3] }
    }

    pub fn x(&self) -> f64 {
        self.e[0]
    }

    pub fn y(&self) -> f64 {
        self.e[1]
    }

    pub fn z(&self) -> f64 {
        self.e[2]
    }

    pub fn length(&self) -> f64 {
        self.length_squared().sqrt()
    }

    pub fn length_squared(&self) -> f64 {
        self.e[0] * self.e[0] + self.e[1] * self.e[1] + self.e[2] * self.e[2]
    }

    pub fn dot(lhs: Vec3, rhs: Vec3) -> f64 {
        lhs.e[0] * rhs.e[0] + lhs.e[1] * rhs.e[1] + lhs.e[2] * rhs.e[2]
    }

    pub fn cross(lhs: Vec3, rhs: Vec3) -> Vec3 {
        Self::new(
            lhs.e[1] * rhs.e[2] - lhs.e[2] * rhs.e[1],
            lhs.e[2] * rhs.e[0] - lhs.e[0] * rhs.e[2],
            lhs.e[0] * rhs.e[1] - lhs.e[1] * rhs.e[0],
        )
    }

    pub fn unit_vector(v: Vec3) -> Vec3 {
        v / v.length()
    }

    pub fn random_vec() -> Vec3 {
        let mut rng = rand::thread_rng();

        Vec3::new(rng.gen(), rng.gen(), rng.gen())
    }

    pub fn random(min: f64, max: f64) -> Vec3 {
        let mut rng = rand::thread_rng();

        Vec3::new(
            rng.gen_range(min..max),
            rng.gen_range(min..max),
            rng.gen_range(min..max),
        )
    }

    pub fn random_in_unit_sphere() -> Vec3 {
        loop {
            let p = Vec3::random(-1.0, 1.0);
            if p.length_squared() >= 1.0 {
                continue;
            }
            return p;
        }
    }

    pub fn random_unit_vector() -> Vec3 {
        Vec3::unit_vector(Vec3::random_in_unit_sphere())
    }

    pub fn random_in_hemisphere(normal: Vec3) -> Vec3 {
        let in_unit_sphere = Vec3::random_in_unit_sphere();
        if Vec3::dot(in_unit_sphere, normal) > 0.0 {
            return in_unit_sphere;
        } else {
            return -in_unit_sphere;
        }
    }

    pub fn near_zero(&self) -> bool {
        let s = 1e-8;
        (self.x().abs() < s) && (self.y().abs() < s) && (self.z().abs() < s)
    }

    pub fn reflect(v: Vec3, n: Vec3) -> Vec3 {
        v - (2.0 * Vec3::dot(v, n)) * n
    }

    pub fn refract(uv: Vec3, n: Vec3, etai_over_etat: f64) -> Vec3 {
        let cos_theta = f64::min(Vec3::dot(-uv, n), 1.0);
        let r_out_perp = etai_over_etat * (uv + cos_theta * n);
        let r_out_parllel = -((1.0 - r_out_perp.length_squared()).abs()).sqrt() * n;
        r_out_perp + r_out_parllel
    }

    pub fn random_in_unit_disk() -> Vec3 {
        let mut rng = rand::thread_rng();

        loop {
            let p = Vec3::new(rng.gen_range(-1.0..1.0), rng.gen_range(-1.0..1.0), 0.0);
            if p.length_squared() >= 1.0 {
                continue;
            }
            return p;
        }
    }
}

impl Default for Vec3 {
    fn default() -> Self {
        Self {
            e: Default::default(),
        }
    }
}

impl ops::Neg for Vec3 {
    type Output = Vec3;

    fn neg(self) -> Self::Output {
        Self::new(-self.e[0], -self.e[1], -self.e[2])
    }
}

impl ops::Index<usize> for Vec3 {
    type Output = f64;

    fn index(&self, index: usize) -> &Self::Output {
        &self.e[index]
    }
}

impl ops::IndexMut<usize> for Vec3 {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        &mut self.e[index]
    }
}

impl ops::AddAssign<Vec3> for Vec3 {
    fn add_assign(&mut self, rhs: Vec3) {
        self.e[0] += rhs.e[0];
        self.e[1] += rhs.e[1];
        self.e[2] += rhs.e[2];
    }
}

impl ops::MulAssign<f64> for Vec3 {
    fn mul_assign(&mut self, rhs: f64) {
        self.e[0] *= rhs;
        self.e[1] *= rhs;
        self.e[2] *= rhs;
    }
}

impl ops::DivAssign<f64> for Vec3 {
    fn div_assign(&mut self, rhs: f64) {
        *self *= (1 as f64) / rhs
    }
}

impl ops::Add<Vec3> for Vec3 {
    type Output = Vec3;

    fn add(self, rhs: Vec3) -> Self::Output {
        Self::Output::new(
            self.e[0] + rhs.e[0],
            self.e[1] + rhs.e[1],
            self.e[2] + rhs.e[2],
        )
    }
}

impl ops::Sub<Vec3> for Vec3 {
    type Output = Vec3;

    fn sub(self, rhs: Vec3) -> Self::Output {
        Self::Output::new(
            self.e[0] - rhs.e[0],
            self.e[1] - rhs.e[1],
            self.e[2] - rhs.e[2],
        )
    }
}

impl ops::Mul<Vec3> for Vec3 {
    type Output = Vec3;

    fn mul(self, rhs: Vec3) -> Self::Output {
        Self::Output::new(
            self.e[0] * rhs.e[0],
            self.e[1] * rhs.e[1],
            self.e[2] * rhs.e[2],
        )
    }
}

impl ops::Mul<f64> for Vec3 {
    type Output = Vec3;

    fn mul(self, rhs: f64) -> Self::Output {
        rhs * self
    }
}

impl ops::Mul<Vec3> for f64 {
    type Output = Vec3;

    fn mul(self, rhs: Vec3) -> Self::Output {
        Self::Output::new(self * rhs.e[0], self * rhs.e[1], self * rhs.e[2])
    }
}

impl ops::Div<f64> for Vec3 {
    type Output = Vec3;

    fn div(self, rhs: f64) -> Self::Output {
        (1 as f64 / rhs) * self
    }
}

impl Display for Vec3 {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{} {} {}", self.x(), self.y(), self.z())
    }
}

pub type Point3 = Vec3;
pub type Color = Vec3;
