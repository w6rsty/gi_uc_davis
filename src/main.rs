use gi::image::Image;
use gi::math::{Color, Point, Vec3};
use gi::ray::{Material, PointLight, Ray, Reflectance};
use gi::scene::Scene;

const WIDTH: u32 = 1920;
const HEIGHT: u32 = 1080;
const ASPECT_RATIO: f32 = WIDTH as f32 / HEIGHT as f32;

fn main() {
    let cube = [
        Point::new(0.5, 0.5, 0.5),    // 0
        Point::new(0.5, 0.5, -0.5),   // 1 
        Point::new(0.5, -0.5, 0.5),   // 2
        Point::new(0.5, -0.5, -0.5),  // 3
        Point::new(-0.5, 0.5, 0.5),   // 4
        Point::new(-0.5, 0.5, -0.5),  // 5
        Point::new(-0.5, -0.5, 0.5),  // 6
        Point::new(-0.5, -0.5, -0.5), // 7
    ];

    let mut scene = Scene::new();
    scene.add_material(Material {
        emissive: false,
        reflectance: Reflectance::None,
        albedo: Vec3::new(1.00, 1.00, 1.00),
        kd: 0.8 / std::f32::consts::PI,
        ks: 0.1,
        shininess: 4,
    });
    scene.add_material(Material {
        emissive: false,
        reflectance: Reflectance::None,
        albedo: Vec3::new(0.56, 0.17, 0.11),
        kd: 0.8 / std::f32::consts::PI,
        ks: 0.1,
        shininess: 16,
    });
    scene.add_material(Material {
        emissive: false,
        reflectance: Reflectance::None,
        albedo: Vec3::new(0.20, 0.34, 0.12),
        kd: 0.8 / std::f32::consts::PI,
        ks: 0.1,
        shininess: 16,
    });
    scene.add_material(Material {
        emissive: false,
        reflectance: Reflectance::Reflection,
        albedo: Vec3::ZERO,
        kd: 0.0 / std::f32::consts::PI,
        ks: 1.0,
        shininess: 32,
    });  
    
    scene.add_traingle(3, cube[6], cube[2], cube[3]); // Bottom
    scene.add_traingle(3, cube[6], cube[3], cube[7]);
    scene.add_traingle(0, cube[0], cube[5], cube[1]); // Top
    scene.add_traingle(0, cube[0], cube[4], cube[5]);
    scene.add_traingle(1, cube[4], cube[6], cube[7]); // Left
    scene.add_traingle(1, cube[4], cube[7], cube[5]);
    scene.add_traingle(2, cube[0], cube[3], cube[2]); // Right
    scene.add_traingle(2, cube[0], cube[1], cube[3]);
    scene.add_traingle(0, cube[5], cube[7], cube[3]); // Back
    scene.add_traingle(0, cube[5], cube[3], cube[1]);

    scene.add_sphere(0, Point::new(-0.2, -0.3,  0.0), 0.2);
    scene.add_sphere(3, Point::new( 0.2, -0.3, -0.2), 0.2);

    scene.add_point_light(PointLight {
        position: Point::new(0.0, 0.3, 0.1),
        intensity: 0.8,
        color: Color::new(1.00, 0.95, 0.90),
    });
    scene.add_point_light(PointLight {
        position: Point::new(-0.4, -0.4, 0.4),
        intensity: 0.8,
        color: Color::new(0.50, 0.50, 0.90),
    });

    let camera_pos = Point::new(-0.2, 0.1, 1.2);
    let look_at = Point::new(0.0, 0.0, 0.0);
    let up = Vec3::new(0.0, 1.0, 0.0);

    let look_dir = (look_at - camera_pos).normalize();
    let u = (look_dir.cross(up)).normalize();
    let v = u.cross(look_dir).normalize();
    
    let mut image = Image::new(WIDTH, HEIGHT);

    for y in 0..HEIGHT {
        for x in 0..WIDTH {
            let fx = (2.0 * (x as f32 + 0.5) / WIDTH as f32 - 1.0) * ASPECT_RATIO;
            let fy = 1.0 - 2.0 * (y as f32 + 0.5) / HEIGHT as f32;
            let view_dir = (look_dir + u * fx + v * fy).normalize();
            let ray = Ray::new(
                camera_pos,
                view_dir
            );

            image.set_pixel(x, y, scene.shade(&ray, 1));
        }
    }

    image.to_png("output.png").unwrap();
}
