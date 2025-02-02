#include "../../include/transformation_plugin.h"

EXPORT struct image transform(const struct image* input_image) {
    struct image transformed = create_image(input_image->height, input_image->width);
    if (!transformed.data) return transformed;
    for (size_t y = 0; y < input_image->height; y++) {
        for (size_t x = 0; x < input_image->width; x++) {
            transformed.data[offset_by_coords(&transformed, y, input_image->width - x - 1)] = input_image->data[offset_by_coords(input_image, x, y)];
        }
    }
    return transformed;
}

EXPORT struct transformation_plugin init_plugin(void) {
    return (struct transformation_plugin) {.key = "cw90", .transform = transform};
}
