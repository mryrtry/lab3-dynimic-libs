#include "image_utils.h"

struct image create_image(uint64_t width, uint64_t height) {
    struct image img = {0};
    img.data = malloc(width * height * sizeof(struct pixel));
    if (!img.data) return img;
    img.width = width;
    img.height = height;
    return img;
}

size_t offset_by_coords(const struct image* image, size_t x, size_t y) {
    return x + y * image->width;
}
