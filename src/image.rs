use std::io::Write;

use crate::math::Color;

pub struct Image {
    width: u32,
    height: u32,
    pixels: Vec<Color>,
}

impl Image {
    pub fn new(width: u32, height: u32) -> Self {
        Image {
            width,
            height,
            pixels: vec![Color::ZERO; (width * height) as usize],
        }
    }

    pub fn set_pixel(&mut self, x: u32, y: u32, color: Color) {
        self.pixels[(y * self.width + x) as usize] = color;
    }

    pub fn get_pixel(&self, x: u32, y: u32) -> Color {
        self.pixels[(y * self.width + x) as usize]
    }

    pub fn to_ppm(&self, filename: &str) -> std::io::Result<()> {
        let mut file = std::fs::File::create(filename)?;
        file.write_fmt(format_args!("P3\n{} {}\n255\n", self.width, self.height))?;
        for y in 0..self.height {
            for x in 0..self.width {
                let color = self.get_pixel(x, y);
                file.write_fmt(format_args!("{}\n", color.to_ppm_color()))?;
            }
        }
        Ok(())
    }

    pub fn to_png(&self, filename: &str) -> std::io::Result<()> {
        let bytes: Vec<u8> = self.pixels.iter()
                               .flat_map(|color| vec![(color.x * 255.0) as u8, (color.y * 255.0) as u8, (color.z * 255.0) as u8, 255u8])
                               .collect();
        image::save_buffer(filename, &bytes, self.width, self.height, image::ExtendedColorType::Rgba8).unwrap();
        Ok(())
    }
}
