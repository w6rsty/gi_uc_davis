#ifndef RAY_H
#define RAY_H

#include "math.h"
#include "ds.h"
#include <stdint.h>

#define MAX_DEPTH 32

typedef struct {
    point_t origin;
    vec3f_t direction;
} ray_t;

point_t ray_at(const ray_t *ray, float t);

typedef struct {
    float       tmin;
    uint32_t    object_id;
    uint32_t    material_id;
    vec3f_t     normal;
    point_t     point;
} shade_record_t;

/* Geometry */

typedef enum hitable_type {
    HITABLE_SPHERE,
    HITABLE_TRIANGLE
} hitable_type_t;

// Generic hitable object
typedef struct {
    uint32_t        id;
    uint32_t        material_id;
    hitable_type_t  type;
} hitable_t;

typedef struct {
    hitable_t   hitable;
    point_t     center;
    float       radius;
} sphere_t;

typedef struct {
    hitable_t   hitable;
    point_t     vertices[3];
} triangle_t;

/* Material */

typedef enum emissive {
    EMISSIVE_FALSE,
    EMISSIVE_TRUE
} emissive_t;

typedef enum reflectance {
    REFLECTANCE_DIFFUSE     = 0x000000001,
    REFLECTANCE_SPECULAR    = 0x000000002,
    REFLECTANCE_REFRACTIVE  = 0x000000004,
} reflectance_t;

typedef struct {
    uint32_t id;
    vec3f_t albedo;
    emissive_t emissive;
    float strength;
    reflectance_t reflectance;
    float roughness;
} material_t;

/* Scene */

typedef struct {
    color_t color;
    point_t position;
} light_t;

typedef struct {
    linked_list_t hitables;
    linked_list_t materials;
    linked_list_t lights;
} scene_t;

void create_scene(scene_t *scene);
void destroy_scene(scene_t *scene);
void add_material(
    vec3f_t albedo,
    emissive_t emissive, float strength,
    reflectance_t reflectance, float roughness,
    scene_t *scene);
// Temporary, will be removed
void add_light(color_t color, point_t position, scene_t *scene);
void add_sphere(point_t center, float radius, uint32_t material_id, scene_t *scene);
void add_triangle(point_t vertices[3], uint32_t material_id, scene_t *scene);

/* Ray hit */

int hit_sphere(const ray_t *ray, const sphere_t *sphere, shade_record_t *srmin);
int hit_triangle(const ray_t *ray, const triangle_t *triangle, shade_record_t *srmin);

int hit_scene(const ray_t *ray, const scene_t *scene, shade_record_t *srmin);
color_t ray_shade(const ray_t *ray, const scene_t *scene, uint32_t depth);

#endif
