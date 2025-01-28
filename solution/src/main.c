#include "bmp.h"
#include "file_utils.h"
#include "image.h"
#include "transform.h"
#include <errno.h>

int main(int argc, char* argv[]) {

    // Проверяем количество аргументов
    if (argc != 4) {
        perror("Not valid argc");
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
        perror("Not valid transform");
        return ENOENT;
    }

    // Открываем входной файл
    FILE* input_file = read_file(input_file_path);
    if (!input_file) return ENOENT;

    // Считываем входное изображение
    struct image input_image;
    enum read_status read_status = from_bmp(input_file, &input_image);

    // Закрываем входной файл
    close_file(input_file);

    // Не удалось считать -> выдаём соответствующую ошибку
    if (read_status != READ_OK) {
        perror("Invalid BMP");
        return read_status;
    }

    // Применяем функцию трансформации
    struct image output_image = transform_func(&input_image);

    // Открываем выходной файл
    FILE* output_file = write_file(output_file_path);
    if (!output_file) return ENOENT;

    // Записываем преобразованное изображение
    enum write_status write_status = to_bmp(output_file, &output_image);

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

    return 0;
}
