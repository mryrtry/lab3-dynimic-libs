#ifndef FORMAT_PLUGIN_H

#define FORMAT_PLUGIN_H

#include "image.h"
#include "read_write_status.h"
#include <stdio.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

enum read_status from_file(FILE* input_file, struct image* image);
enum write_status to_file(FILE* output_file, struct image* image);

struct format_plugin {
    char* extension;
    enum read_status (*from_file)(FILE* input_file, struct image* image);
    enum write_status (*to_file)(FILE* output_file, struct image* image);
};

EXPORT struct format_plugin init_plugin(void);

#endif
