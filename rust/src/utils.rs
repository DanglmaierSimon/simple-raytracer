use crate::vec3::Color;
use std::{f64::consts::PI, io::Write};

pub const INFINITY: f64 = std::f64::MAX;

pub fn deg_to_rad(degrees: f64) -> f64 {
    degrees * PI / 180.0
}

pub fn write_color(
    out: &mut impl Write,
    pixel_color: Color,
    samples_per_pixel: u32,
) -> std::io::Result<()> {
    let (mut r, mut g, mut b) = (pixel_color.x(), pixel_color.y(), pixel_color.z());

    // divide the color by the number of samples and gamma correct for gamma=2.0
    let scale = 1.0 / samples_per_pixel as f64;
    r = (scale * r).sqrt();
    g = (scale * g).sqrt();
    b = (scale * b).sqrt();

    // write translated [0,255] value of each color component
    write!(
        out,
        "{} {} {}\n",
        (256.0 * clamp(r, 0.0, 0.999)) as u8,
        (256.0 * clamp(g, 0.0, 0.999)) as u8,
        (256.0 * clamp(b, 0.0, 0.999)) as u8
    )
}

pub fn clamp(x: f64, min: f64, max: f64) -> f64 {
    if x < min {
        min
    } else if x > max {
        max
    } else {
        x
    }
}
