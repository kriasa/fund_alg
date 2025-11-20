#ifndef BASE_CONVERSION_H
#define BASE_CONVERSION_H

#include <stddef.h>

typedef enum {
    CONV_SUCCESS = 0,
    CONV_ERR_NULL_PTR = 1,
    CONV_ERR_INVALID_BASE = 2,
    CONV_ERR_BUFFER_TOO_SMALL = 3,
    CONV_ERR_OVERFLOW = 4
} ConversionStatus;

ConversionStatus decimal_to_base2r(int decimal_num, int r, char* result, size_t result_size);
char digit_to_char(int digit);

#endif