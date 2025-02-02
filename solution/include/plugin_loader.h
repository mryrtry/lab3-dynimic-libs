#ifndef PLUGIN_H

#define PLUGIN_H

#include "read_write_status.h"
#include <stdio.h>
#include "image.h"

#define MAX_PLUGINS 20

typedef struct image (*transform_func)(const struct image*);
typedef enum read_status (*from_file_func)(FILE* input_file, struct image* image);
typedef enum write_status (*to_file_func)(FILE* output_file, struct image* image);
typedef struct transformation_plugin (*init_transform_plugin)(void);
typedef struct format_plugin (*init_format_plugin)(void);

struct format_plugin {
    char* extension;
    enum read_status (*from_file)(FILE* input_file, struct image* image);
    enum write_status (*to_file)(FILE* output_file, struct image* image);
};

struct transformation_plugin {
    char* key;
    struct image (*transform)(const struct image* source);
};

extern void* dll_handlers[MAX_PLUGINS * 2];
extern struct transformation_plugin transform_plugins[MAX_PLUGINS];
extern struct format_plugin format_plugins[MAX_PLUGINS];
extern size_t transform_plugins_count;
extern size_t format_plugins_count;

void load_transform_plugins(const char* plugins_directory);
void load_format_plugins(const char* plugins_directory);
void free_plugins(void);

#endif
