#include "../../include/transformation_plugin.h"

EXPORT struct image transform(const struct image* source) {
    return *source;
}

EXPORT struct transformation_plugin init_plugin(void) {
    return (struct transformation_plugin) {.key = "none", .transform = transform};
}
