#include "image.h"
#include "math.h"
#include "ray.h"

static int Width = 1200;
static int Height = 800;
static vec3f_t CameraPos = {0.0f, 0.0f, 1.1f};
static float ScreenDistance = 1.0f;

int main(int argc, char *argv[]) {
    point_t cube[8] = {
        { 0.5f,  0.5f,  0.5f}, // 0
        { 0.5f,  0.5f, -0.5f}, // 1
        { 0.5f, -0.5f,  0.5f}, // 2
        { 0.5f, -0.5f, -0.5f}, // 3
        {-0.5f,  0.5f,  0.5f}, // 4
        {-0.5f,  0.5f, -0.5f}, // 5
        {-0.5f, -0.5f,  0.5f}, // 6
        {-0.5f, -0.5f, -0.5f}, // 7
    };

    // Bottom
    point_t face_0[3] = {cube[7], cube[6], cube[3]};
    point_t face_1[3] = {cube[6], cube[2], cube[3]};
    // Right
    point_t face_2[3] = {cube[0], cube[3], cube[2]};
    point_t face_3[3] = {cube[0], cube[1], cube[3]};
    // Left
    point_t face_4[3] = {cube[4], cube[6], cube[7]};
    point_t face_5[3] = {cube[4], cube[7], cube[5]};
    // Top
    point_t face_6[3] = {cube[0], cube[5], cube[1]};
    point_t face_7[3] = {cube[0], cube[4], cube[5]};
    // Back
    point_t face_8[3] = {cube[5], cube[7], cube[3]};
    point_t face_9[3] = {cube[5], cube[3], cube[1]};

    point_t face_10[3] = {{ 0.2f,  0.49f,  0.1f}, {-0.2f,  0.49f, -0.1f}, { 0.2f,  0.49f, -0.1f}};
    point_t face_11[3] = {{ 0.2f,  0.49f,  0.1f}, {-0.2f,  0.49f,  0.1f}, {-0.2f,  0.49f, -0.1f}};

    scene_t scene;
    create_scene(&scene);
    add_material((vec3f_t){1.00f, 1.00f, 1.00f}, EMISSIVE_FALSE, &scene); // 0
    add_material((vec3f_t){0.56f, 0.17f, 0.11f}, EMISSIVE_FALSE, &scene); // 1
    add_material((vec3f_t){0.20f, 0.34f, 0.12f}, EMISSIVE_FALSE, &scene); // 2
    add_material((vec3f_t){0.80f, 0.34f, 0.12f}, EMISSIVE_FALSE, &scene); // 3
    add_material((vec3f_t){1.00f, 0.00f, 0.00f}, EMISSIVE_TRUE, &scene);  // 4
    add_light((vec3f_t){1.00f, 0.90f, 0.50f}, (point_t){ 0.4f, -0.4f,  0.3f}, &scene);
    // add_light((vec3f_t){0.50f, 0.50f, 0.90f}, (point_t){-0.4f, -0.4f, -0.3f}, &scene);

    add_sphere((point_t){-0.2f, -0.4f, 0.3f}, 0.1f, 0, &scene); // 0
    add_sphere((point_t){ 0.1f, -0.3f, 0.0f}, 0.2f, 0, &scene); // 1
    add_triangle(face_0, 0, &scene); // 2
    add_triangle(face_1, 0, &scene); // 3
    add_triangle(face_2, 2, &scene); // 4
    add_triangle(face_3, 2, &scene); // 5
    add_triangle(face_4, 1, &scene); // 6
    add_triangle(face_5, 1, &scene); // 7
    add_triangle(face_6, 0, &scene); // 8
    add_triangle(face_7, 0, &scene); // 9
    add_triangle(face_8, 0, &scene); // 10
    add_triangle(face_9, 0, &scene); // 11

    image_t image;
    create_image(Width, Height, &image);
    float apect_ratio = Width / (float)Height;

    for (int y = 0; y < Height; ++y) {
        for (int x = 0; x < Width; ++x) {
            ray_t ray = {};
            ray.origin = CameraPos;
            float fx = (2.0f * (x + 0.5f) / (float)Width - 1.0f) * apect_ratio;
            float fy = 1.0f - 2.0f * (y + 0.5f) / (float)Height;
            vec3f_t screen_pos = {fx, fy, CameraPos.z - ScreenDistance};
            vec3f_t direction = sub_vec3f(&screen_pos, &CameraPos);
            ray.direction = normalize(&direction);

            color_t color = ray_shade(&ray, &scene);
            set_pixel_image(&image, x, y, color_to_uint32(&color));
        }
    }

    write_image_ppm("output.ppm", &image);
    
    destroy_image(&image);
    destroy_scene(&scene);
    return 0;
}
