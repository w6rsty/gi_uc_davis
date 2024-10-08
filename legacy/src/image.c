#include "image.h"

#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

void create_image(uint32_t width, uint32_t height, image_t *image) {
    image->width = width;
    image->height = height;
    image->data = malloc(width * height * sizeof(uint32_t));
    memset(image->data, 0, width * height * sizeof(uint32_t));
}

void destroy_image(image_t *image) {
    free(image->data);
    image->data = NULL;
    image->width = 0;
    image->height = 0;
}

void set_pixel_image(image_t *image, uint32_t x, uint32_t y, uint32_t color) {
    image->data[y * image->width + x] = color;
}

uint32_t read_pixel_image(const image_t *image, uint32_t x, uint32_t y) {
    return image->data[y * image->width + x];
}

void write_image_ppm(const char *filename, const image_t *image) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "P3\n");
    fprintf(file, "%d %d\n", image->width, image->height);
    fprintf(file, "255\n");

    for (int i = 0;  i < image->width * image->height; ++i) {
        uint32_t *pixel = image->data + i;
        unsigned char r = *pixel >> 24;
        unsigned char g = *pixel >> 16;
        unsigned char b = *pixel >> 8; 
        fprintf(file, "%d %d %d\n", r, g, b);
    }

    fclose(file);
}
