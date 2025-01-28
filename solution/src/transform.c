#include "transform.h"

struct transform_entry transforms[] = {
{"none", none},
{"cw90", rotate_90_cw},
{"ccw90", rotate_90_ccw},
{"fliph", flip_horizontal},
{"flipv", flip_vertical},
};

struct image none(const struct image* input_image) {
    return *input_image;
}

struct image rotate_90_ccw(const struct image* input_image) {
    struct image transformed = create_image(input_image->height, input_image->width);
    for (uint64_t y = 0; y < input_image->height; y++) {
        for (uint64_t x = 0; x < input_image->width; x++) {
            transformed.data[x * transformed.width + (input_image->height - y - 1)] = input_image->data[y * input_image->width + x];
        }
    }
    return transformed;
}

struct image rotate_90_cw(const struct image* input_image) {
    struct image transformed = create_image(input_image->height, input_image->width);
    for (uint64_t y = 0; y < input_image->height; y++) {
        for (uint64_t x = 0; x < input_image->width; x++) {
            transformed.data[(input_image->width - x - 1) * transformed.width + y] = input_image->data[y * input_image->width + x];
        }
    }
    return transformed;
}

struct image flip_horizontal(const struct image* input_image) {
    struct image transformed = create_image(input_image->width, input_image->height);
    for (uint64_t y = 0; y < input_image->height; y++) {
        for (uint64_t x = 0; x < input_image->width; x++) {
            transformed.data[y * transformed.width + (input_image->width - x - 1)] = input_image->data[y * input_image->width + x];
        }
    }
    return transformed;
}

struct image flip_vertical(const struct image* input_image) {
    struct image transformed = create_image(input_image->width, input_image->height);
    for (uint64_t y = 0; y < input_image->height; y++) {
        for (uint64_t x = 0; x < input_image->width; x++) {
            transformed.data[(input_image->height - y - 1) * transformed.width + x] = input_image->data[y * input_image->width + x];
        }
    }
    return transformed;
}

transform_func find_transform(const char* name) {
    if (name == NULL) return NULL;
    for (size_t i = 0; i < sizeof(transforms) / sizeof(transforms[0]); i++) {
        if (strcmp(transforms[i].name, name) == 0) {
            return transforms[i].func;
        }
    }
    return NULL;
}
