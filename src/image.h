#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

typedef struct {
    uint32_t *data;
    uint32_t width;
    uint32_t height;
} image_t;

void create_image(uint32_t width, uint32_t height, image_t *image);
void destroy_image(image_t *image);
void set_pixel_image(image_t *image, uint32_t x, uint32_t y, uint32_t color);
uint32_t read_pixel_image(const image_t *image, uint32_t x, uint32_t y);

// Write image to a PPM file
void write_image_ppm(const char *filename, const image_t *image);

#endif
