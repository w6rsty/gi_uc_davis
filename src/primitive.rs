use core::f32;

use crate::math::Point;
use crate::ray::{Hitable, Ray, ShaderRecord};

pub struct Sphere {
    pub material_id: usize,
    pub center: Point,
    pub radius: f32,
}

impl Sphere {
    pub fn new(material_id: usize, center: Point, radius: f32) -> Self {
        Sphere {
            material_id,
            center,
            radius,
        }
    }
}

impl Hitable for Sphere {
    fn hit(&self, ray: &Ray, tmin: f32) -> Option<(f32, ShaderRecord)> {
        let oc = ray.origin - self.center;
        let a = ray.direction.length_squared();
        let half_b = oc.dot(ray.direction);
        let c = oc.length_squared() - self.radius * self.radius;
        let discriminant = half_b * half_b - a * c;

        if discriminant < 0.0 {
            return None;
        }

        let sqrtd = discriminant.sqrt();

        let mut root = (-half_b - sqrtd) / a;
        if root < 0.0 || root > tmin {
            root = (-half_b + sqrtd) / a;
            if root < 0.0 || root > tmin {
                return None;
            }
        }

        let point = ray.at(root);
        let normal = (point - self.center) / self.radius;
        Some((
            root,
            ShaderRecord {
                point: point + normal * 0.0001,
                normal,
                hitable_id: 0,
                material_id: self.material_id,
            },
        ))
    }
}

pub struct Triangle {
    pub material_id: usize,
    pub a: Point,
    pub b: Point,
    pub c: Point,
}

impl Triangle {
    pub fn new(material_id: usize, a: Point, b: Point, c: Point) -> Self {
        Triangle {
            material_id,
            a,
            b,
            c,
        }
    }
}

impl Hitable for Triangle {
    fn hit(&self, ray: &Ray, tmin: f32) -> Option<(f32, ShaderRecord)> {
        let edge1 = self.b - self.a;
        let edge2 = self.c - self.a;
        let ray_cross_e2 = ray.direction.cross(edge2);
        let det = edge1.dot(ray_cross_e2);

        if det.abs() < 1e-6f32 {
            return None;
        }

        let inv_det = 1.0 / det;
        let s = ray.origin - self.a;
        let u = inv_det * s.dot(ray_cross_e2);

        if u < 0.0 || u > 1.0 {
            return None;
        }

        let _cross_e1 = s.cross(edge1);
        let v = inv_det * ray.direction.dot(_cross_e1);

        if v < 0.0 || u + v > 1.0 {
            return None;
        }

        let t = inv_det * edge2.dot(_cross_e1);

        if t < 0.0 || t > tmin {
            return None;
        }
        
        let point = ray.at(t);
        let normal = edge1.cross(edge2).normalize();
        Some((
            t,
            ShaderRecord {
                point: point + normal * 0.0001,
                normal,
                hitable_id: 1,
                material_id: self.material_id,
            },
        ))
    }
}
