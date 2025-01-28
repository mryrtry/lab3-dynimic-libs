#ifndef TRANSFORM_H

#define TRANSFORM_H

#include <string.h>
#include "image.h"

typedef struct image (*transform_func)(const struct image*);

struct transform_entry {
    const char* name;
    transform_func func;
};

extern struct transform_entry transforms[];

transform_func find_transform(const char* name);

struct image none(const struct image* input_image);
struct image rotate_90_ccw(const struct image* input_image);
struct image rotate_90_cw(const struct image* input_image);
struct image flip_horizontal(const struct image* input_image);
struct image flip_vertical(const struct image* input_image);

#endif
