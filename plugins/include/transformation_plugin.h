#ifndef PLUGINS_TRANSFORMATION_PLUGIN_H

#define PLUGINS_TRANSFORMATION_PLUGIN_H

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

#include "transform_utils.h"
#include "image.h"

struct image transform(const struct image* source);

struct transformation_plugin {
    char* key;
    struct image (*transform)(const struct image* source);
};

EXPORT struct transformation_plugin init_plugin(void);

#endif
