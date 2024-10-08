use crate::math::{Point, Color};
use crate::primitive::{Triangle, Sphere};
use crate::ray::{Hitable, Material, PointLight, Ray, ShaderRecord};

pub struct Scene {
    pub hitables: Vec<Box<dyn Hitable>>,
    pub materials: Vec<Material>,
    pub point_lights: Vec<PointLight>,
}

impl Scene {
    pub fn new() -> Self {
        Scene {
            hitables: Vec::new(),
            materials: Vec::new(),
            point_lights: Vec::new(),
        }
    }

    pub fn add_traingle(&mut self, material_id: usize, a: Point, b: Point, c: Point) {
        self.hitables.push(Box::new(Triangle::new(material_id, a, b, c)));
    }

    pub fn add_sphere(&mut self, material_id: usize, center: Point, radius: f32) {
        self.hitables.push(Box::new(Sphere::new(material_id, center, radius)));
    }

    pub fn add_hitable(&mut self, hitable: Box<dyn Hitable>) {
        self.hitables.push(hitable);
    }

    pub fn add_material(&mut self, material: Material) {
        self.materials.push(material);
    }

    pub fn add_point_light(&mut self, point_light: PointLight) {
        self.point_lights.push(point_light);
    }

    pub fn shade(&self, ray: &Ray) -> Color {
        if let Some((_t, record)) = self.hit_all(ray, f32::INFINITY) {
            self.materials[record.material_id].albedo
        } else {
            Color::ZERO
        }
    }

    fn hit_all(&self, ray: &Ray, tmin: f32) -> Option<(f32, ShaderRecord)> {
        let mut tmin = tmin;
        let mut result = None;
        for hitable in self.hitables.iter() {
            if let Some(record) = hitable.hit(ray, tmin) {
                if record.0 < tmin {
                    tmin = record.0;
                    result = Some(record)
                }
            }
        }

        result
    }
}
