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
pub enum Reflectance {
    None,
    Reflection,
    Refraction
}

#[derive(Debug)]
pub struct Material {
    pub emissive: bool,
    pub reflectance: Reflectance,
    pub albedo: Vec3,
    pub kd: f32,
    pub ks: f32,
    pub shininess: u32,
}

impl Default for Material {
    fn default() -> Self {
        Material {
            emissive: false,
            reflectance: Reflectance::None,
            albedo: Vec3::new(1.0, 1.0, 1.0),
            kd: 1.0,
            ks: 0.1,
            shininess: 32,
        }
    }
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
