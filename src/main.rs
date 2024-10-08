use gi::image::Image;
use gi::math::{Color, Point, Vec3};
use gi::ray::{Material, PointLight, Ray};
use gi::scene::Scene;

const WIDTH: u32 = 800;
const HEIGHT: u32 = 600;
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
        albedo: Vec3::new(1.00, 1.00, 1.00),
    });
    scene.add_material(Material {
        albedo: Vec3::new(0.56, 0.17, 0.11),
    });
    scene.add_material(Material {
        albedo: Vec3::new(0.20, 0.34, 0.12),
    });
    
    scene.add_traingle(0, cube[7], cube[6], cube[3]); // Bottom
    scene.add_traingle(0, cube[6], cube[2], cube[3]);
    scene.add_traingle(0, cube[0], cube[5], cube[1]); // Top
    scene.add_traingle(0, cube[0], cube[4], cube[5]);
    scene.add_traingle(1, cube[4], cube[6], cube[7]); // Left
    scene.add_traingle(1, cube[4], cube[7], cube[5]);
    scene.add_traingle(2, cube[0], cube[3], cube[2]); // Right
    scene.add_traingle(2, cube[0], cube[1], cube[3]);
    scene.add_traingle(0, cube[5], cube[7], cube[3]); // Back
    scene.add_traingle(0, cube[5], cube[3], cube[1]);

    scene.add_sphere(1, Point::new(0.0, 0.0, 0.0), 0.1);

    scene.add_point_light(PointLight {
        position: Point::new(0.0, 0.0, 0.0),
        intensity: 1.0,
        color: Color::new(1.0, 1.0, 1.0),
    });

    let camera_pos: Point = Point::new(0.0, 0.0, 1.1);
    let screen_distance: f32 = 1.0;

    let mut image = Image::new(WIDTH, HEIGHT);

    for y in 0..HEIGHT {
        for x in 0..WIDTH {
            let fx = (2.0 * (x as f32 + 0.5) / WIDTH as f32 - 1.0) * ASPECT_RATIO;
            let fy = 1.0 - 2.0 * (y as f32 + 0.5) / HEIGHT as f32;
            let ray = Ray::new(
                camera_pos,
                (Vec3::new(fx, fy, camera_pos.z - screen_distance) - camera_pos).normalize(),
            );

            image.set_pixel(x, y, scene.shade(&ray));
        }
    }

    image.to_png("output.png").unwrap();
}
