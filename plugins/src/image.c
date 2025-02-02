#include "../include/image.h"

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
