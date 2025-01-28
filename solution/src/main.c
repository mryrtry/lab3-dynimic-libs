#include "image.h"
#include "bmp.h"
#include "transform.h"

int main(int argc, char* argv[]) {

    // Проверяем количество аргументов
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <source-image> <output_image-image> <transformation>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Распределяем аргументы
    const char* input_file_path = argv[1];
    const char* output_file_path = argv[2];
    const char* transform = argv[3];

    // Ищём функцию трансформации в таблице
    transform_func transform_func = find_transform(transform);

    // На нашли функцию -> ENOENT
    if (!transform_func) {
        fprintf(stderr, "Unknown transform: %s\n", transform);
        return ENOENT;
    }

    // Открываем входной файл
    FILE* input_file = fopen(input_file_path, "rb");

    // Не удалось открыть файл -> ENOENT
    if (!input_file) {
        fprintf(stderr, "Failed to open input file\n");
        return ENOENT;
    }

    // Считываем входное изображение
    struct image input_image;
    enum read_status read_status = from_bmp(input_file, &input_image);

    // Закрываем входной файл
    fclose(input_file);

    // Не удалось считать -> выдаём соответствующую ошибку
    if (read_status != READ_OK) {
        fprintf(stderr, "Failed to read BMP file\n");
        return read_status;
    }

    // Применяем функцию трансформации
    struct image output_image = transform_func(&input_image);

    // Открываем выходной файл
    FILE* output_file = fopen(output_file_path, "wb");

    // Не удалось открыть файл -> ENOENT
    if (!output_file) {
        free_heap(&input_image, &output_image);
        fprintf(stderr, "Failed to open output file\n");
        return ENOENT;
    }

    // Записываем преобразованное изображение
    enum write_status write_status = to_bmp(output_file, &output_image);

    // Закрываем выходной файл
    fclose(output_file);

    // Не удалось записать -> выдаём соответствующую ошибку
    if (write_status != WRITE_OK) {
        free_heap(&input_image, &output_image);
        fprintf(stderr, "Failed to write BMP file\n");
        return write_status;
    }

    // Очищаем выделенную в куче память
    free_heap(&input_image, &output_image);

    return 0;
}
