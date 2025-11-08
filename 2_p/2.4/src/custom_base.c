#include "../include/custom_base.h"
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

static int digit_value(char c, int base, bool uppercase) {
    if (c >= '0' && c <= '9') {
        int val = c - '0';
        return (val < base) ? val : -1;
    }
   
    if (uppercase) {
        if (c >= 'A' && c <= 'Z') {
            int val = c - 'A' + 10;
            return (val < base) ? val : -1;
        }
    } else {
        if (c >= 'a' && c <= 'z') {
            int val = c - 'a' + 10;
            return (val < base) ? val : -1;
        }
    }
   
    return -1;
}

scanf_status_t parse_custom_base(const char* str, int base, bool uppercase, int* result, int* const consumed_chars) {
    if (!str || !result || !consumed_chars) {
        return SCANF_INVALID_FORMAT;
    }

    if (base < 2 || base > 36) {
        base = 10;
    }

    *consumed_chars = 0;
   
    const char* ptr = str;
    while (*ptr && isspace((unsigned char)*ptr)) {
        ptr++;
    }

    if (*ptr == '\0') {
        return BASE_INVALID_CHAR;
    }
   
    int sign = 1;

    if (*ptr == '-') {
        sign = -1;
        ptr++;
    } else if (*ptr == '+') {
        ptr++;
    }
   
    long total = 0;
    int digits_found = 0;
    const char* start = ptr;

    while (*ptr) {
        int digit = digit_value(*ptr, base, uppercase);
        if (digit == -1) {
            break;
        }

        if (total > (LONG_MAX - digit) / base) {
            *consumed_chars = (int)(ptr - str);
            return BASE_OVERFLOW;
        }
       
        total = total * base + digit;
        digits_found++;
        ptr++;
    }

    if (digits_found == 0) {
        *consumed_chars = (int)(start - str);
        return BASE_INVALID_CHAR;
    }

    if (sign == -1) {
        total = -total;
    }

    if (total > INT_MAX || total < INT_MIN) {
        *consumed_chars = (int)(ptr - str);
        return BASE_OVERFLOW;
    }
   
    *result = (int)total;
    *consumed_chars = (int)(ptr - str);
    return SCANF_SUCCESS;
}