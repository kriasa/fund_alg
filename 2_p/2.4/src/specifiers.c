#include "../include/specifiers.h"
#include "../include/roman_numbers.h"
#include "../include/zeckendorf.h"
#include "../include/custom_base.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

specifier_info_t parse_specifier(const char** format) {
    specifier_info_t info = {SPECIFIER_NONE, 0, 10};
   
    if (!format || !*format) return info;
   
    if (strncmp(*format, "Ro", 2) == 0) {
        info.type = SPECIFIER_ROMAN;
        *format += 2;
    } else if (strncmp(*format, "Zr", 2) == 0) {
        info.type = SPECIFIER_ZECKENDORF;
        *format += 2;
    } else if (strncmp(*format, "Cv", 2) == 0) {
        info.type = SPECIFIER_BASE_LOWER;
        *format += 2;
    } else if (strncmp(*format, "CV", 2) == 0) {
        info.type = SPECIFIER_BASE_UPPER;
        *format += 2;
    } else if (isalpha((unsigned char)**format) || **format == '%') {
        info.type = SPECIFIER_STANDARD;
        info.standard_char = **format;
        (*format)++;
    } else {
        info.type = SPECIFIER_NONE;
    }
   
    return info;
}

scanf_status_t handle_specifier(specifier_info_t spec_info, const char* buffer, va_list* args, int* const consumed_chars) {
    if (!buffer || !args || !consumed_chars) return SCANF_INVALID_FORMAT;
   
    *consumed_chars = 0;
   
    switch (spec_info.type) {
        case SPECIFIER_ROMAN: {
            int* ptr = va_arg(*args, int*);
            return parse_roman_number(buffer, ptr, consumed_chars);
        }
       
        case SPECIFIER_ZECKENDORF: {
            unsigned int* ptr = va_arg(*args, unsigned int*);
            return parse_zeckendorf(buffer, ptr, consumed_chars);
        }
       
        case SPECIFIER_BASE_LOWER:
        case SPECIFIER_BASE_UPPER: {
            int* ptr = va_arg(*args, int*);
            int base = va_arg(*args, int);
            bool uppercase = (spec_info.type == SPECIFIER_BASE_UPPER);
            return parse_custom_base(buffer, base, uppercase, ptr, consumed_chars);
        }
       
        case SPECIFIER_STANDARD: {
            char format_str[4] = "% ";
            format_str[1] = spec_info.standard_char;
           
            void* ptr = va_arg(*args, void*);
            int result = sscanf(buffer, format_str, ptr);
           
            if (result == 1) {
                *consumed_chars = (int)strlen(buffer);
                return SCANF_SUCCESS;
            } else {
                return SCANF_NO_MATCH;
            }
        }
       
        default:
            return SCANF_INVALID_FORMAT;
    }
}