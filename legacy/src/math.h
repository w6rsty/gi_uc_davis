#ifndef MATH_H
#define MATH_H

#include <stdint.h>

typedef struct {
    float x;
    float y;
    float z;
} vec3f_t;

typedef vec3f_t color_t;
typedef vec3f_t point_t;

vec3f_t add_vec3f(const vec3f_t *a, const vec3f_t *b);
vec3f_t add_scalar_vec3f(const vec3f_t *a, float b);
vec3f_t sub_vec3f(const vec3f_t *a, const vec3f_t *b);
vec3f_t sub_scalar_vec3f(const vec3f_t *a, float b);
vec3f_t mul_vec3f(const vec3f_t *a, const vec3f_t *b);
vec3f_t mul_scalar_vec3f(const vec3f_t *a, float b);
float dot_vec3f(const vec3f_t *a, const vec3f_t *b);
vec3f_t cross_vec3f(const vec3f_t *a, const vec3f_t *b);
float length_vec3f(const vec3f_t *a);
vec3f_t neg_vec3f(const vec3f_t *a);

vec3f_t normalize(const vec3f_t *a);
vec3f_t reflect(const vec3f_t *v, const vec3f_t *normal);

void format_vec3f(const vec3f_t *a);

float clamp(float x, float min, float max);
uint32_t color_to_uint32(const color_t *color);

float random_float(float min, float max);
float max_float(float a, float b);
float min_float(float a, float b);

#endif
