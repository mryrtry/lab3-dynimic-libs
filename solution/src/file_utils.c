#include "file_utils.h"

FILE* read_file(const char* file_path) {
    FILE *file = fopen(file_path, "rb");
    if (!file) fprintf(stderr, "Invalid file %s", file_path);
    return file;
}

FILE* write_file(const char* file_path) {
    FILE *file = fopen(file_path, "wb");
    if (!file) fprintf(stderr, "Invalid file %s", file_path);
    return file;
}

void close_file(FILE* file) {
    if (!file) perror("Closing file is NULL");
    if (fclose(file)) perror("Can't close file");
}
