use crate::math::{Color, Point, Vec3};

#[derive(Debug, Clone, Copy)]
pub struct Ray {
    pub origin: Point,
    pub direction: Vec3,
}

impl Ray {
    pub fn new(origin: Vec3, direction: Vec3) -> Self {
        Ray { origin, direction }
    }

    pub fn at(&self, t: f32) -> Point {
        self.origin + self.direction * t
    }
}

#[derive(Debug)]
pub struct Material {
    pub albedo: Vec3,
}

#[derive(Debug, Clone, Copy)]
pub struct PointLight {
    pub position: Point,
    pub intensity: f32,
    pub color: Color,
}

#[derive(Debug, Clone, Copy)]
pub struct ShaderRecord {
    pub point: Point,
    pub normal: Vec3,
    pub hitable_id: usize,
    pub material_id: usize,
}

impl Default for ShaderRecord {
    fn default() -> Self {
        ShaderRecord {
            point: Point::ZERO,
            normal: Vec3::ZERO,
            hitable_id: 0,
            material_id: 0,
        }
    }
}

pub trait Hitable {
    fn hit(&self, ray: &Ray, tmin: f32) -> Option<(f32, ShaderRecord)>;
}
