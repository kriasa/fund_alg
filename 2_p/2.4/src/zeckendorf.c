#include "../include/zeckendorf.h"
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

static unsigned int zeckendorf_fibonacci(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n == 3) return 2;
   
    unsigned int a = 1;
    unsigned int b = 2;
   
    for (int i = 4; i <= n; i++) {
        unsigned int c = a + b;
        if (c < a || c < b) {
            return UINT_MAX;
        }
        a = b;
        b = c;
    }
    return b;
}

bool is_valid_zeckendorf_char(char c) {
    return c == '0' || c == '1';
}

scanf_status_t parse_zeckendorf(const char* str, unsigned int* result, int* const consumed_chars) {
    if (!str || !result || !consumed_chars) {
        return SCANF_INVALID_FORMAT;
    }
   
    *consumed_chars = 0;
   
    const char* ptr = str;
    int len = 0;

    while (ptr[len] && is_valid_zeckendorf_char(ptr[len])) {
        len++;
    }
   
    if (len < 2) {
        *consumed_chars = len;
        return ZECK_INVALID_SEQUENCE;
    }

    if (ptr[len-1] != '1') {
        *consumed_chars = len;
        return ZECK_INVALID_SEQUENCE;
    }
    for (int i = 0; i < len - 2; i++) {
        if (ptr[i] == '1' && ptr[i+1] == '1') {
            *consumed_chars = len;
            return ZECK_INVALID_SEQUENCE;
        }
    }

    unsigned int total = 0;
    for (int i = len - 2; i >= 0; i--) {
        if (ptr[i] == '1') {
            int fib_index = len - i;
           
            unsigned int fib_val = zeckendorf_fibonacci(fib_index);
           
            if (fib_val == UINT_MAX) {
                *consumed_chars = len;
                return ZECK_OVERFLOW;
            }
           
            if (total > UINT_MAX - fib_val) {
                *consumed_chars = len;
                return ZECK_OVERFLOW;
            }
           
            total += fib_val;
        }
    }
   
    *result = total;
    *consumed_chars = len;
    return SCANF_SUCCESS;
}