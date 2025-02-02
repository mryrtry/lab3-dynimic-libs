#include "../../include/transformation_plugin.h"

EXPORT struct image transform(const struct image* source) {
    struct image transformed = create_image(source->height, source->width);
    if (!transformed.data) return transformed;
    for (size_t y = 0; y < source->height; y++) {
        for (size_t x = 0; x < source->width; x++) {
            transformed.data[offset_by_coords(&transformed, source->height - y - 1, x)] = source->data[offset_by_coords(source, x, y)];
        }
    }
    return transformed;
}

EXPORT struct transformation_plugin init_plugin(void) {
    return (struct transformation_plugin) {.key = "ccw90", .transform = transform};
}
