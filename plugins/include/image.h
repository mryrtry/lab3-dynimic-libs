#ifndef PLUGINS_IMAGE_H

#define PLUGINS_IMAGE_H

#include <stdint.h>
#include <stdlib.h>

struct pixel {
    uint8_t b, g, r;
};

struct image {
    uint64_t width, height;
    struct pixel* data;
};

#endif
