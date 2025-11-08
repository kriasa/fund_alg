#ifndef SPECIFIERS_H
#define SPECIFIERS_H

#include "error_codes.h"
#include <stdarg.h>

typedef enum {
    SPECIFIER_ROMAN = 1,
    SPECIFIER_ZECKENDORF = 2,
    SPECIFIER_BASE_LOWER = 3,
    SPECIFIER_BASE_UPPER = 4,
    SPECIFIER_STANDARD = 5,
    SPECIFIER_NONE = -1
} specifier_type_t;

typedef struct {
    specifier_type_t type;
    char standard_char;
    int base;
} specifier_info_t;

specifier_info_t parse_specifier(const char** format);
scanf_status_t handle_specifier(specifier_info_t spec_info, const char* buffer, va_list* args, int* const consumed_chars);

#endif