#include "math.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

vec3f_t add_vec3f(const vec3f_t *a, const vec3f_t *b) {
    vec3f_t result;
    result.x = a->x + b->x;
    result.y = a->y + b->y;
    result.z = a->z + b->z;
    return result;
}

vec3f_t add_scalar_vec3f(const vec3f_t *a, float b) {
    vec3f_t result;
    result.x = a->x + b;
    result.y = a->y + b;
    result.z = a->z + b;
    return result;
}

vec3f_t sub_vec3f(const vec3f_t *a, const vec3f_t *b) {
    vec3f_t result;
    result.x = a->x - b->x;
    result.y = a->y - b->y;
    result.z = a->z - b->z;
    return result;
}

vec3f_t sub_scalar_vec3f(const vec3f_t *a, float b) {
    vec3f_t result;
    result.x = a->x - b;
    result.y = a->y - b;
    result.z = a->z - b;
    return result;
}

vec3f_t mul_vec3f(const vec3f_t *a, const vec3f_t *b) {
    vec3f_t result;
    result.x = a->x * b->x;
    result.y = a->y * b->y;
    result.z = a->z * b->z;
    return result;
}

vec3f_t mul_scalar_vec3f(const vec3f_t *a, float b) {
    vec3f_t result;
    result.x = a->x * b;
    result.y = a->y * b;
    result.z = a->z * b;
    return result;
}

float dot_vec3f(const vec3f_t *a, const vec3f_t *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

vec3f_t cross_vec3f(const vec3f_t *a, const vec3f_t *b) {
    vec3f_t result;
    result.x = a->y * b->z - a->z * b->y;
    result.y = a->z * b->x - a->x * b->z;
    result.z = a->x * b->y - a->y * b->x;
    return result;
}

float length_vec3f(const vec3f_t *a) {
    return sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}

vec3f_t normalize(const vec3f_t *a) {
    vec3f_t result;
    float length = sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
    if (length != 0) {
        result.x = a->x / length;
        result.y = a->y / length;
        result.z = a->z / length;
    } else {
        result.x = result.y = result.z = 0;
    }
    return result;
}

void format_vec3f(const vec3f_t *a) {
    printf("(%f, %f, %f)\n", a->x, a->y, a->z);
}

float clamp(float x, float min, float max) {
    if (x < min) {
        return min;
    } else if (x > max) {
        return max;
    } else {
        return x;
    }
}

uint32_t color_to_uint32(const color_t *color) {
    uint32_t r = (uint32_t)(clamp(color->x, 0.0f, 1.0f) * 255.0f);
    uint32_t g = (uint32_t)(clamp(color->y, 0.0f, 1.0f) * 255.0f);
    uint32_t b = (uint32_t)(clamp(color->z, 0.0f, 1.0f) * 255.0f);
    return (r << 24) | (g << 16) | (b << 8) | 255;
}

float random_float(float min, float max) {
    float scale = rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}

float max_float(float a, float b) {
    return a > b ? a : b;
}

float min_float(float a, float b) {
    return a < b ? a : b;
}