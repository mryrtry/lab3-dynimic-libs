#ifndef FILE_UTILS_H

#define FILE_UTILS_H

#include "stdio.h"

FILE* read_file(const char* file_path);
FILE* write_file(const char* file_path);
void close_file(FILE* file);

#endif
