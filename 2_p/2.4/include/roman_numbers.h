#ifndef ROMAN_NUMBERS_H
#define ROMAN_NUMBERS_H

#include "error_codes.h"
#include <stdbool.h>

bool is_valid_roman_char(char c);
scanf_status_t parse_roman_number(const char* str, int* result, int* const consumed_chars);

#endif