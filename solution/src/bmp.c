#include "bmp_utils.h"
#include "bmp.h"
#include <errno.h>


// Чтение bmp
enum read_status from_bmp(FILE* input_file, struct image* image) {
    struct bmp_header header = {0};

    enum read_status validation_status = validate_bmp_header(input_file, &header);
    if (validation_status != READ_OK) return validation_status;

    image->width = header.biWidth;
    image->height = header.biHeight;
    image->data = malloc(image->width * image->height * sizeof(struct pixel));

    uint32_t padding = count_padding(image->width);

    for (uint32_t y = 0; y < image->height; y++) {
        for (uint32_t x = 0; x < image->width; x++) {
            if (fread(&image->data[y * image->width + x], sizeof(struct pixel), 1, input_file) != 1) {
                free(image->data);
                return ENOMEM;
            }
        }
        fseek(input_file, (long) padding, SEEK_CUR);
    }

    return READ_OK;
}

// Запись bmp
enum write_status to_bmp(FILE* output_file, const struct image* image) {
    struct bmp_header header = create_bmp_header(*image);

    if (!fwrite(&header, sizeof(header), 1, output_file)) {
        return WRITE_ERROR;
    }

    uint32_t padding = count_padding(image->width);
    size_t pad_byte = 0;
    for (uint32_t y = 0; y < image->height; y++) {
        for (uint32_t x = 0; x < image->width; x++) {
            if (!fwrite(&image->data[y * image->width + x], sizeof(struct pixel), 1, output_file)) {
                return WRITE_ERROR;
            }
        }
        if (fwrite(&pad_byte, 1, padding, output_file) != padding) {
            return WRITE_ERROR;
        }
    }

    return WRITE_OK;
}
