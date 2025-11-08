#ifndef ZECKENDORF_H
#define ZECKENDORF_H

#include "error_codes.h"
#include <stdbool.h>

bool is_valid_zeckendorf_char(char c);
scanf_status_t parse_zeckendorf(const char* str, unsigned int* result, int* const consumed_chars);

#endif