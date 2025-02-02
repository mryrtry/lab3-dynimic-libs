#include "plugin_loader.h"
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#define PLUGIN_EXTENSION ".dll"
#define PLUGIN_FUNCTION "init_plugin"
#else
#include <dlfcn.h>
#define LoadLibrary(name)            dlopen(name, RTLD_LAZY | RTLD_GLOBAL)
#define FreeLibrary(dll)             dlclose(dll)
#define GetProcAddress(dll, symbol)  dlsym(dll, symbol)
#define PLUGIN_EXTENSION ".so"
#define PLUGIN_FUNCTION "init_plugin"
#endif

void* dll_handlers[MAX_PLUGINS * 2] = {0};
struct transformation_plugin transform_plugins[MAX_PLUGINS] = {0};
struct format_plugin format_plugins[MAX_PLUGINS] = {0};
size_t transform_plugins_count = 0;
size_t format_plugins_count = 0;

void load_transform_plugin(const char* plugin_path) {
    if (transform_plugins_count >= MAX_PLUGINS) {
        fprintf(stderr, "Plugin %s wasn't load, max count of plugins exists", plugin_path);
        return;
    }

    void* plugin_handle;

    // Загружаем библиотеку
    plugin_handle = LoadLibrary(plugin_path);
    if (!plugin_handle) {
        fprintf(stderr, "Failed to load plugin: %s\n", plugin_path);
        return;
    }

    // Получаем init_plugin
    struct transformation_plugin (*init_plugin)(void) = (struct transformation_plugin (*)(void))GetProcAddress(plugin_handle, PLUGIN_FUNCTION);

    // Нет init_plugin -> выходим
    if (!init_plugin) {
        fprintf(stderr, "Plugin %s does not have %s function\n", plugin_path, PLUGIN_FUNCTION);
        FreeLibrary(plugin_handle);
        return;
    }

    // Получаем структуру плагина
    struct transformation_plugin plugin = init_plugin();
    transform_plugins[transform_plugins_count] = plugin;
    dll_handlers[transform_plugins_count] = plugin_handle;
    transform_plugins_count++;
}

void load_format_plugin(const char* plugin_path) {
    if (format_plugins_count >= MAX_PLUGINS) {
        fprintf(stderr, "Plugin %s wasn't load, max count of plugins exists", plugin_path);
        return;
    }

    void* plugin_handle;

    // Загружаем библиотеку
    plugin_handle = LoadLibrary(plugin_path);
    if (!plugin_handle) {
        fprintf(stderr, "Failed to load plugin: %s\n", plugin_path);
        return;
    }

    // Получаем init_plugin
    struct format_plugin (*init_plugin)(void) = (struct format_plugin (*)(void))GetProcAddress(plugin_handle, PLUGIN_FUNCTION);

    // Нет init_plugin -> выходим
    if (!init_plugin) {
        fprintf(stderr, "Plugin %s does not have %s function\n", plugin_path, PLUGIN_FUNCTION);
        FreeLibrary(plugin_handle);
        return;
    }

    // Получаем структуру плагина
    struct format_plugin plugin = init_plugin();
    format_plugins[format_plugins_count] = plugin;
    dll_handlers[format_plugins_count] = plugin_handle;
    format_plugins_count++;
}

void load_transform_plugins(const char* plugins_dir) {
    char search_dir_transform[256];
    snprintf(search_dir_transform, sizeof(search_dir_transform), "%s/transform", plugins_dir);
    char search_path[256];
    snprintf(search_path, sizeof(search_path), "%s/*%s", search_dir_transform, PLUGIN_EXTENSION);

    #ifdef _WIN32
    WIN32_FIND_DATA find_file_data;
    HANDLE hFind = FindFirstFile(search_path, &find_file_data);

    if (hFind == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Failed to open plugins directory: %s\n", search_dir_transform);
        return;
    }

    do {
        char plugin_path[256];
        snprintf(plugin_path, sizeof(plugin_path), "%s/%s", search_dir_transform, find_file_data.cFileName);
        load_transform_plugin(plugin_path);
    } while (FindNextFile(hFind, &find_file_data) != 0);

    FindClose(hFind);
    #else
        DIR* dir = opendir(search_dir_transform);
        if (!dir) {
            perror("Failed to open plugins directory");
            return;
        }

        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strstr(entry->d_name, PLUGIN_EXTENSION) != NULL) {
                char plugin_path[256];
                snprintf(plugin_path, sizeof(plugin_path), "%s/%s", search_dir_transform, entry->d_name);
                load_transform_plugin(plugin_path);
            }
        }

        closedir(dir);
    #endif
}

void load_format_plugins(const char* plugins_dir) {
    char search_dir_format[256];
    snprintf(search_dir_format, sizeof(search_dir_format), "%s/format", plugins_dir);
    char search_path[256];
    snprintf(search_path, sizeof(search_path), "%s/*%s", search_dir_format, PLUGIN_EXTENSION);

    #ifdef _WIN32
    WIN32_FIND_DATA find_file_data;
    HANDLE hFind = FindFirstFile(search_path, &find_file_data);

    if (hFind == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Failed to open plugins directory: %s\n", search_dir_format);
        return;
    }

    do {
        char plugin_path[256];
        snprintf(plugin_path, sizeof(plugin_path), "%s/%s", search_dir_format, find_file_data.cFileName);
        load_format_plugin(plugin_path);
    } while (FindNextFile(hFind, &find_file_data) != 0);

    FindClose(hFind);
    #else
    DIR* dir = opendir(search_dir_format);
    if (!dir) {
        perror("Failed to open plugins directory");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, PLUGIN_EXTENSION) != NULL) {
            char plugin_path[256];
            snprintf(plugin_path, sizeof(plugin_path), "%s/%s", search_dir_format, entry->d_name);
            load_format_plugin(plugin_path);
        }
    }

    closedir(dir);
    #endif
}

void free_plugins(void) {
    for (size_t i = 0; i < format_plugins_count + transform_plugins_count; i++) {
        FreeLibrary(dll_handlers[i]);
    }
}
