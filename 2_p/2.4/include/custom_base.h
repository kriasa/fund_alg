#ifndef CUSTOM_BASE_H
#define CUSTOM_BASE_H

#include "error_codes.h"
#include <stdbool.h>

scanf_status_t parse_custom_base(const char* str, int base, bool uppercase, int* result, int* const consumed_chars);

#endif