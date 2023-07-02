use std::{fs::File, io::Write};

#[derive(Copy, Clone, Debug, PartialEq)]
pub enum PpmImageType {
    RawPpm,
    PlainPpm,
}

pub struct PpmImage {
    width: usize,
    height: usize,
    image_type: PpmImageType,
    pixels: Vec<(u8, u8, u8)>, // r, g, b
}

impl PpmImage {
    pub fn new(width: usize, height: usize, image_type: PpmImageType) -> Self {
        assert!(
            image_type == PpmImageType::PlainPpm,
            "only plain ppm images supported for now"
        );

        let v = vec![(0, 0, 0); width * height];

        PpmImage {
            width,
            height,
            image_type,
            pixels: v,
        }
    }

    pub fn set_value(&mut self, idx: usize, color: (u8, u8, u8)) {
        let px = self.pixels.get_mut(idx).expect("index out of range");
        *px = color;
    }

    pub fn write_to_file(&self, file: File) -> std::io::Result<()> {
        let mut file = file;

        let header = format!(
            "{}\n{} {}\n255\n",
            magic_number(self.image_type),
            self.width,
            self.height
        );

        file.write_all(header.as_bytes())?;

        for pixels in &self.pixels {
            writeln!(file, "{} {} {}", pixels.0, pixels.1, pixels.2)?;
        }

        file.flush()?;
        Ok(())
    }

    pub fn width(&self) -> usize {
        self.width
    }

    pub fn height(&self) -> usize {
        self.height
    }
}

const fn magic_number(ppm_type: PpmImageType) -> &'static str {
    match ppm_type {
        PpmImageType::RawPpm => "P6",
        PpmImageType::PlainPpm => "P3",
    }
}
