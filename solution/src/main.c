#include "file_utils.h"
#include <errno.h>
#include <string.h>
#include "plugin_loader.h"

const char* get_file_extension(const char* filename) {
    const char* dot = strrchr(filename, '.');
    if (dot && dot != filename) {
        return dot;
    }
    return NULL;
}

int main(int argc, char* argv[]) {

    // Проверяем количество аргументов
    if (argc != 5) {
        perror("Not valid argc");
        return EXIT_FAILURE;
    }

    // Распределяем аргументы
    const char* plugin_build = argv[1];
    const char* input_file_path = argv[2];
    const char* output_file_path = argv[3];
    const char* transform = argv[4];

    // Получаем расширение input file
    const char* input_extension = get_file_extension(input_file_path);
    if (!input_extension) {
        perror("Invalid extension");
        return ENOENT;
    }

    // Получаем расширение output file
    const char* output_extension = get_file_extension(output_file_path);
    if (!output_extension) {
        perror("Invalid extension");
        return ENOENT;
    }

    // Если расширения разные -> ENOENT
    if (strcmp(output_extension, input_extension) != 0) {
        perror("Input file extension != Output file extension");
        return ENOENT;
    }

    // Подгружаем плагины
    load_transform_plugins(plugin_build);
    load_format_plugins(plugin_build);

    // Ищем функции загрузки выгрузки из файла
    to_file_func to_file_func = NULL;
    from_file_func from_file_func = NULL;
    for (size_t i = 0; i < format_plugins_count; i++) {
        struct format_plugin plugin = format_plugins[i];
        if (!strcmp(input_extension, plugin.extension)) {
            from_file_func = plugin.from_file;
            to_file_func = plugin.to_file;
            break;
        }
    }

    // Не нашли функции -> ENOENT
    if (!to_file_func || !from_file_func) {
        perror("Not valid format");
        return ENOENT;
    }

    // Ищем функцию трансформации
    transform_func transform_func = NULL;
    for (size_t i = 0; i < transform_plugins_count; i++) {
        struct transformation_plugin plugin = transform_plugins[i];
        if (!strcmp(transform, plugin.key)) {
            transform_func = plugin.transform;
            break;
        }
    }

    // На нашли функцию -> ENOENT
    if (!transform_func) {
        perror("Not valid transform");
        return ENOENT;
    }

    // Открываем входной файл
    FILE* input_file = read_file(input_file_path);

    if (!input_file) return ENOENT;

    // Считываем входное изображение
    struct image input_image;
    enum read_status read_status = from_file_func(input_file, &input_image);

    // Закрываем входной файл
    close_file(input_file);

    // Не удалось считать -> выдаём соответствующую ошибку
    if (read_status != READ_OK) {
        perror("Invalid BMP");
        return read_status;
    }

    // Применяем функцию трансформации
    struct image output_image = transform_func(&input_image);
    if (!output_image.data) return ENOMEM;

    // Открываем выходной файл
    FILE* output_file = write_file(output_file_path);
    if (!output_file) return ENOENT;

    // Записываем преобразованное изображение
    enum write_status write_status = to_file_func(output_file, &output_image);

    // Закрываем выходной файл
    close_file(output_file);

    // Не удалось записать -> выдаём соответствующую ошибку
    if (write_status != WRITE_OK) {
        free_heap(&input_image, &output_image);
        perror("Failed to write BMP file");
        return write_status;
    }

    // Очищаем выделенную в куче память
    free_heap(&input_image, &output_image);

    // Закрываем dll
    free_plugins();

    return 0;
}
