#ifndef IO_STATUS_H

#define IO_STATUS_H

enum read_status {
    READ_OK = 0,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER,
    READ_MEMORY_ERROR
};

enum write_status {
    WRITE_OK = 0,
    WRITE_ERROR
};

#endif

