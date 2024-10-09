use crate::math::{direction_reflect, Color, Point};
use crate::primitive::{Triangle, Sphere};
use crate::ray::{Hitable, Material, PointLight, Ray, Reflectance, ShaderRecord};

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

    pub fn shade(&self, ray: &Ray, depth: u32) -> Color {
        let mut result = Color::ZERO;
        if depth > 64 {
            return result;
        }        
        if let Some((_, record)) = self.hit_all(ray, f32::INFINITY) {
            let material = &self.materials[record.material_id];
            if ray.direction.dot(record.normal) > 0.0 {
                return result;
            }
            
            for light in self.point_lights.iter() {
                let distance = (light.position - record.point).length();
                let mut stmin = f32::INFINITY;
                let shadow_ray = Ray::new(
                    record.point,
                    (light.position - record.point).normalize()
                );
                if let Some((st, _)) = self.hit_all(&shadow_ray, stmin) {
                    stmin = st;
                }
                if stmin > distance {
                    let h = (light.position - record.point).normalize();
                    let blinn_phong = record.normal.dot(h).max(0.0).powi(32) / record.normal.dot(ray.direction).abs();
                    let cos = record.normal.dot((light.position - record.point).normalize()).max(0.0);
                    result += material.albedo * (blinn_phong * material.ks + material.kd) * cos
                                              * light.color * light.intensity;
                }
            }
            match material.reflectance {
                Reflectance::Reflection => {
                    if record.normal.dot(-ray.direction) > 0.0 {
                        let reflect_ray = Ray::new(
                            record.point,
                            direction_reflect(-ray.direction, record.normal)
                        );
                        let att: f32 = 0.7;
                        result += self.shade(&reflect_ray, depth + 1) * att.powi(depth as i32);
                    }
                },
                _ => {}
            }


            result += material.albedo * 0.2; // ambient
        }

        result
    }

    /// Find a closest hit
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
