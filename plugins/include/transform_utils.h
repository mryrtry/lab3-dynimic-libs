#ifndef PLUGIN_TRANSFORM_UTILS_H

#define PLUGIN_TRANSFORM_UTILS_H

#include "image.h"
#include <stdlib.h>
#include <stdint.h>

struct image create_image(uint64_t width, uint64_t height);
size_t offset_by_coords(const struct image* image, size_t x, size_t y);

#endif
