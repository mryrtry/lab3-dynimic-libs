#include "../../include/format_plugin.h"
#include <errno.h>

#define BMP_FTYPE 0x4D42
#define BMP_SIZE 40
#define BMP_PLANES 1
#define BMP_BIT_COUNT 24
#define BYTES_PER_PIXEL 3
#define BMP_ALIGNMENT 4
#define PIXEL_SIZE sizeof(struct pixel)

struct __attribute__((packed)) bmp_header {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

EXPORT enum read_status validate_bmp_header(FILE* input_file, struct bmp_header* header) {
    if (!fread(header, sizeof(struct bmp_header), 1, input_file)) {
        return READ_INVALID_HEADER;
    }
    if (header->bfType != BMP_FTYPE) {
        return READ_INVALID_SIGNATURE;
    }
    if (header->biBitCount != BMP_BIT_COUNT) {
        return READ_INVALID_BITS;
    }
    return READ_OK;
}

EXPORT struct bmp_header create_bmp_header(struct image image) {
    struct bmp_header header = {0};
    header.bfType = BMP_FTYPE;
    header.biSize = BMP_SIZE;
    header.bfileSize = sizeof(struct bmp_header) + image.width * image.height * sizeof(struct pixel);
    header.bOffBits = sizeof(struct bmp_header);
    header.biWidth = image.width;
    header.biHeight = image.height;
    header.biPlanes = BMP_PLANES;
    header.biBitCount = BMP_BIT_COUNT;
    header.biSizeImage = image.width * image.height * sizeof(struct pixel);
    return header;
}

static uint32_t count_padding(uint32_t width) {
    return (BMP_ALIGNMENT - (width * BYTES_PER_PIXEL) % BMP_ALIGNMENT) % BMP_ALIGNMENT;
}

enum read_status from_file(FILE* input_file, struct image* image) {
    if (!image) return ENOMEM;

    struct bmp_header header = {0};

    enum read_status validation_status = validate_bmp_header(input_file, &header);
    if (validation_status != READ_OK) return validation_status;

    image->width = header.biWidth;
    image->height = header.biHeight;
    if (image->width < 0 || image->height < 0) return READ_INVALID_BITS;
    image->data = malloc(image->width * image->height * PIXEL_SIZE);
    if (!image->data) return ENOMEM;

    uint8_t padding = count_padding(image->width);

    for (size_t y = 0; y < image->height; y++) {
        for (size_t x = 0; x < image->width; x++) {
            if (!fread(&image->data[y * image->width + x], PIXEL_SIZE, 1, input_file)) {
                free(image->data);
                return ENOMEM;
            }
        }
        fseek(input_file, (long) padding, SEEK_CUR);
    }

    return READ_OK;
}

enum write_status to_file(FILE* output_file, struct image* image) {
    struct bmp_header header = create_bmp_header(*image);

    if (!fwrite(&header, sizeof(header), 1, output_file)) {
        return WRITE_ERROR;
    }

    uint8_t padding = count_padding(image->width);
    size_t pad_byte = 0;
    for (size_t y = 0; y < image->height; y++) {
        for (size_t x = 0; x < image->width; x++) {
            if (!fwrite(&image->data[y * image->width + x], PIXEL_SIZE, 1, output_file)) {
                return WRITE_ERROR;
            }
        }
        if (fwrite(&pad_byte, 1, padding, output_file) != padding) {
            return WRITE_ERROR;
        }
    }

    return WRITE_OK;
}

EXPORT struct format_plugin init_plugin(void) {
    return (struct format_plugin) {.extension = ".bmp", .from_file = from_file, .to_file = to_file};
}
