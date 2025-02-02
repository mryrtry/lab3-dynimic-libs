#ifndef IMAGE_UTILS_H

#define IMAGE_UTILS_H

#include "stdlib.h"
#include "image.h"
#include <stdint.h>

struct image create_image(uint64_t width, uint64_t height);
size_t offset_by_coords(const struct image* image, size_t x, size_t y);

#endif
