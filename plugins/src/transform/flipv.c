#include "../../include/transformation_plugin.h"

EXPORT struct image transform(const struct image* source) {
    struct image transformed = create_image(source->width, source->height);
    if (!transformed.data) return transformed;
    for (size_t y = 0; y < source->height; y++) {
        for (size_t x = 0; x < source->width; x++) {
            transformed.data[offset_by_coords(&transformed, x, source->height - y - 1)] = source->data[offset_by_coords(source, x, y)];
        }
    }
    return transformed;
}

EXPORT struct transformation_plugin init_plugin(void) {
    return (struct transformation_plugin) {.key = "flipv", .transform = transform};
}
