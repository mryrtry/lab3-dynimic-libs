#include "image.h"

// Создание объекта image
struct image create_image(uint64_t width, uint64_t height) {
    struct image img;
    img.width = width;
    img.height = height;
    img.data = malloc(width * height * sizeof(struct pixel));
    return img;
}

// Освобождение кучи
void free_image(struct image* image) {
    free(image->data);
}

// Освобождение кучи
void free_heap(struct image* input_image, struct image* output_image) {
    free_image(input_image);
    if (output_image->data != input_image->data) {
        free_image(output_image);
    }
}
