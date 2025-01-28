#ifndef BMP_UTILS_H

#define BMP_UTILS_H

#include "bmp.h"
#include <stdio.h>

enum read_status validate_bmp_header(FILE* input_file, struct bmp_header* header);
struct bmp_header create_bmp_header(struct image image);
uint32_t count_padding(uint32_t width);

#endif
