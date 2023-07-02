use rand::{thread_rng, Rng};

pub fn clamp<T: std::cmp::PartialOrd>(x: T, min: T, max: T) -> T {
    if x < min {
        min
    } else if x > max {
        max
    } else {
        x
    }
}

pub fn random_in_range(min: f64, max: f64) -> f64 {
    let mut rng = thread_rng();

    rng.gen_range(min..max)
}
