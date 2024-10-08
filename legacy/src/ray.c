#include "ray.h"
#include "math.h"
#include <math.h>
#include <stdlib.h>

point_t ray_at(const ray_t *ray, float t) {
    point_t point = mul_scalar_vec3f(&ray->direction, t);
    return add_vec3f(&ray->origin, &point);
}

void create_scene(scene_t *scene) {
    init_linked_list(&scene->hitables);
    init_linked_list(&scene->materials);
    init_linked_list(&scene->lights);
}

void destroy_scene(scene_t *scene) {
    clear_linked_list(&scene->hitables);
    clear_linked_list(&scene->materials);
    clear_linked_list(&scene->lights);
}

void add_material(
    vec3f_t albedo,
    emissive_t emissive, float strength,
    reflectance_t reflectance, float roughness,
    scene_t *scene) {
    material_t *material = malloc(sizeof(material_t));
    material->id = scene->materials.size;
    material->albedo = albedo;
    material->emissive = emissive;
    material->strength = strength;
    material->reflectance = reflectance;
    material->roughness = roughness;
    insert_back_linked_list(&scene->materials, material);
}

void add_light(color_t color, point_t position, scene_t *scene) {
    light_t *light = malloc(sizeof(light_t));
    light->color = color;
    light->position = position;
    insert_back_linked_list(&scene->lights, light);
}

void add_sphere(point_t center, float radius, uint32_t material_id, scene_t *scene) {
    sphere_t *sphere = malloc(sizeof(sphere_t));
    sphere->hitable.id = scene->hitables.size;
    sphere->hitable.material_id = material_id;
    sphere->hitable.type = HITABLE_SPHERE;
    sphere->center = center;
    sphere->radius = radius;
    insert_back_linked_list(&scene->hitables, sphere);
}

void add_triangle(point_t vertices[3], uint32_t material_id, scene_t *scene) {
    triangle_t *triangle = malloc(sizeof(triangle_t));
    triangle->hitable.id = scene->hitables.size;
    triangle->hitable.material_id = material_id;
    triangle->hitable.type = HITABLE_TRIANGLE;
    for (int i = 0; i < 3; i++) {
        triangle->vertices[i] = vertices[i];
    }
    insert_back_linked_list(&scene->hitables, triangle);
}

int hit_sphere(const ray_t *ray, const sphere_t *sphere, shade_record_t *srmin) {
    float a = dot_vec3f(&ray->direction, &ray->direction);
    vec3f_t oc = sub_vec3f(&ray->origin, &sphere->center);
    float half_b = dot_vec3f(&oc, &ray->direction);
    float c = dot_vec3f(&oc, &oc) - sphere->radius * sphere->radius;
    float discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return -1;
    }
    float sqrtd = sqrtf(discriminant);
    float root = (-half_b - sqrtd) / a;
    if (root < 0) {
        root = (-half_b + sqrtd) / a;
    }

    if (root >= 0 && root < srmin->tmin) {
        srmin->tmin = root;
        srmin->point = ray_at(ray, root);
        vec3f_t normal = sub_vec3f(&srmin->point, &sphere->center);
        srmin->normal = normalize(&normal);
        srmin->object_id = sphere->hitable.id;
        srmin->material_id = sphere->hitable.material_id;
        return 1;
    }

    return -1;
}

int hit_triangle(const ray_t *ray, const triangle_t *triangle, shade_record_t *srmin) {
    vec3f_t edge1 = sub_vec3f(&triangle->vertices[1], &triangle->vertices[0]);
    vec3f_t edge2 = sub_vec3f(&triangle->vertices[2], &triangle->vertices[0]);
    vec3f_t ray_cross_e2 = cross_vec3f(&ray->direction, &edge2);
    float det = dot_vec3f(&edge1, &ray_cross_e2);

    if (fabs(det) < 1e-6) {
        return -1;
    }

    float inv_det = 1.0f / det;
    vec3f_t s = sub_vec3f(&ray->origin, &triangle->vertices[0]);
    float u = dot_vec3f(&s, &ray_cross_e2) * inv_det;

    if (u < 0.0f || u > 1.0f) {
        return -1;
    }

    vec3f_t s_cross_e1 = cross_vec3f(&s, &edge1);
    float v = dot_vec3f(&ray->direction, &s_cross_e1) * inv_det;

    if (v < 0.0f || u + v > 1.0f) {
        return -1;
    }

    float t = dot_vec3f(&edge2, &s_cross_e1) * inv_det;

    if (t < srmin->tmin && t > 0) {
        srmin->tmin = t;
        srmin->point = ray_at(ray, t);
        vec3f_t normal = cross_vec3f(&edge1, &edge2);
        srmin->normal = normalize(&normal);
        srmin->object_id = triangle->hitable.id;
        srmin->material_id = triangle->hitable.material_id;
        return 1;
    } else {
        return -1;
    }
}

int hit_scene(const ray_t *ray, const scene_t *scene, shade_record_t *srmin) {
    int result = -1;

    for (int i = 0; i < scene->hitables.size; ++i) {
        hitable_t *hitable = get_linked_list(&scene->hitables, i);
        switch (hitable->type) {
            case HITABLE_SPHERE: {
                if (hit_sphere(ray, (sphere_t*)(hitable), srmin) != -1) {
                    result = 1;
                }
                break;
            }
            case HITABLE_TRIANGLE: {
                if (hit_triangle(ray, (triangle_t*)(hitable), srmin) != -1) {
                    result = 1;
                }
                break;
            }
        }
    }
    return result;
}

color_t ray_shade(const ray_t *ray, const scene_t *scene, uint32_t depth) {
    if (++depth > MAX_DEPTH) {
        return (color_t){0, 0, 0};
    }
    color_t color = {0.01f, 0.01f, 0.01f};
    shade_record_t srmin = {};
    srmin.tmin = 10000;
    if (hit_scene(ray, scene, &srmin) == -1) {
        return color;
    }

    material_t *material = get_linked_list(&scene->materials, srmin.material_id);
    // Add shading model here

    if (material->emissive == EMISSIVE_TRUE) {
        color = material->albedo;
    } else {
        ray_t shadow_ray = {};
        vec3f_t bias = mul_scalar_vec3f(&srmin.normal, 0.001f);
        shadow_ray.origin = add_vec3f(&srmin.point, &bias);
        for (int i = 0; i < scene->lights.size; ++i) {
            light_t *light = get_linked_list(&scene->lights, i);
            vec3f_t light_direction = sub_vec3f(&light->position, &srmin.point);
            float distance = length_vec3f(&light_direction);
            shadow_ray.direction = normalize(&light_direction);
            shade_record_t shadow_sr = {};
            shadow_sr.tmin = 10000;
            hit_scene(&shadow_ray, scene, &shadow_sr);
            if (shadow_sr.tmin > distance) {
                float light_intensity = 0.5f * max_float(0.0f, dot_vec3f(&shadow_ray.direction, &srmin.normal));
                color_t light_color = mul_scalar_vec3f(&light->color, light_intensity);
                light_color = mul_vec3f(&light_color, &material->albedo);
                color = add_vec3f(&color, &light_color);
            }
        }
        color_t ambient = mul_scalar_vec3f(&material->albedo, 0.1f);
        color = add_vec3f(&color, &ambient);
    }
    if (material->reflectance &= REFLECTANCE_SPECULAR) {
        ray_t reflected_ray = {};
        reflected_ray.origin = srmin.point;
        vec3f_t reverse_v = neg_vec3f(&ray->direction);
        reflected_ray.direction = reflect(&reverse_v, &srmin.normal);

        color_t reflected_color = ray_shade(&reflected_ray, scene, depth);
        reflected_color = mul_vec3f(&reflected_color, &material->albedo);
        reflected_color = mul_scalar_vec3f(&reflected_color, material->roughness);
        color = add_vec3f(&color, &reflected_color);
    }
    // Depth
    // color_t ambient = {1, 1, 1};
    // color = mul_scalar_vec3f(&ambient, 1.0f - 1.0f / (srmin.tmax + 1.0f));

    return color;
}