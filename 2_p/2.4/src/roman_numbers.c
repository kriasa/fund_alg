#include "../include/roman_numbers.h"
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>

typedef struct {
    const char* symbol;
    int value;
    int max_repeat;
    bool is_subtractive;
} roman_token_t;

static const roman_token_t roman_tokens[] = {
    {"M", 1000, 3, false}, {"CM", 900, 1, true}, {"D", 500, 1, false}, {"CD", 400, 1, true},
    {"C", 100, 3, false}, {"XC", 90, 1, true}, {"L", 50, 1, false}, {"XL", 40, 1, true},
    {"X", 10, 3, false}, {"IX", 9, 1, true}, {"V", 5, 1, false}, {"IV", 4, 1, true},
    {"I", 1, 3, false}
};
static const int roman_tokens_count = sizeof(roman_tokens) / sizeof(roman_tokens[0]);

bool is_valid_roman_char(char c) {
    c = (char)toupper((unsigned char)c);
    return c == 'I' || c == 'V' || c == 'X' || c == 'L' || c == 'C' || c == 'D' || c == 'M';
}

scanf_status_t parse_roman_number(const char* str, int* result, int* const consumed_chars) {
    if (!str || !result || !consumed_chars) {
        return SCANF_INVALID_FORMAT;
    }
   
    *consumed_chars = 0;
    if (str[0] == '\0') {
        return ROMAN_INVALID_SEQUENCE;
    }
   
    int total = 0;
    int prev_value = INT_MAX;
    int char_repeat_count = 0;
    char last_repeatable_char = '\0';
   
    bool v_used = false, l_used = false, d_used = false;
    bool was_subtractive = false;
   
    const char* ptr = str;
    const char* start_ptr = str;

    while (*ptr) {
        const roman_token_t* matched_token = NULL;
        int token_len = 0;
       
        for (int i = 0; i < roman_tokens_count; i++) {
            size_t sym_len = strlen(roman_tokens[i].symbol);
            if (strncasecmp(ptr, roman_tokens[i].symbol, sym_len) == 0) {
                matched_token = &roman_tokens[i];
                token_len = (int)sym_len;
                break;
            }
        }
       
        if (matched_token == NULL) {
            if (!is_valid_roman_char(*ptr)) {
                *consumed_chars = (int)(ptr - start_ptr);
                return ROMAN_INVALID_CHAR;
            }
            *consumed_chars = (int)(ptr - start_ptr);
            return ROMAN_INVALID_SEQUENCE;
        }

        // 1. Проверка порядка
        if (matched_token->value > prev_value) {
            *consumed_chars = (int)(ptr - start_ptr);
            return ROMAN_INVALID_SEQUENCE;
        }

        // 2. Запрет вычитания после V, L, D
        if (matched_token->is_subtractive && (prev_value == 5 || prev_value == 50 || prev_value == 500)) {
            *consumed_chars = (int)(ptr - start_ptr);
            return ROMAN_INVALID_SEQUENCE;
        }
       
        // 3. ПРОВЕРКА ВЫЧИТАНИЯ: после вычитающей пары нельзя ставить символы того же разряда
        if (was_subtractive) {
            // Простая логика: после IX/IV нельзя I или V
            if ((prev_value == 9 || prev_value == 4) &&
                (matched_token->symbol[0] == 'I' || matched_token->symbol[0] == 'V')) {
                *consumed_chars = (int)(ptr - start_ptr);
                return ROMAN_INVALID_SEQUENCE;
            }
            // После XC/XL нельзя X или L
            if ((prev_value == 90 || prev_value == 40) &&
                (matched_token->symbol[0] == 'X' || matched_token->symbol[0] == 'L')) {
                *consumed_chars = (int)(ptr - start_ptr);
                return ROMAN_INVALID_SEQUENCE;
            }
            // После CM/CD нельзя C или D
            if ((prev_value == 900 || prev_value == 400) &&
                (matched_token->symbol[0] == 'C' || matched_token->symbol[0] == 'D')) {
                *consumed_chars = (int)(ptr - start_ptr);
                return ROMAN_INVALID_SEQUENCE;
            }
        }
       
        // 4. V, L, D: проверка на повторное использование
        if (strchr(matched_token->symbol, 'V') || strchr(matched_token->symbol, 'v')) {
            if (v_used) {
                *consumed_chars = (int)(ptr - start_ptr);
                return ROMAN_INVALID_SEQUENCE;
            }
            v_used = true;
        }
        if (strchr(matched_token->symbol, 'L') || strchr(matched_token->symbol, 'l')) {
            if (l_used) {
                *consumed_chars = (int)(ptr - start_ptr);
                return ROMAN_INVALID_SEQUENCE;
            }
            l_used = true;
        }
        if (strchr(matched_token->symbol, 'D') || strchr(matched_token->symbol, 'd')) {
            if (d_used) {
                *consumed_chars = (int)(ptr - start_ptr);
                return ROMAN_INVALID_SEQUENCE;
            }
            d_used = true;
        }

        // 5. Повторение I, X, C, M
        if (!matched_token->is_subtractive) {
            char current_base_char = matched_token->symbol[0];
           
            if (current_base_char == 'I' || current_base_char == 'X' ||  
                current_base_char == 'C' || current_base_char == 'M') {
               
                if (current_base_char == last_repeatable_char) {
                    char_repeat_count++;
                    if (char_repeat_count > matched_token->max_repeat) {
                        *consumed_chars = (int)(ptr - start_ptr);
                        return ROMAN_INVALID_SEQUENCE;
                    }
                } else {
                    last_repeatable_char = current_base_char;
                    char_repeat_count = 1;
                }
            } else {
                last_repeatable_char = '\0';
                char_repeat_count = 0;
            }
        } else {
            last_repeatable_char = '\0';
            char_repeat_count = 0;
        }
       
        // 6. Проверка переполнения
        if ((long)total + matched_token->value > INT_MAX) {
            *consumed_chars = (int)(ptr - start_ptr);
            return ROMAN_OVERFLOW;
        }
       
        total += matched_token->value;
        prev_value = matched_token->value;
        was_subtractive = matched_token->is_subtractive;
        ptr += token_len;
    }
   
    *result = total;
    *consumed_chars = (int)(ptr - start_ptr);
    return SCANF_SUCCESS;
}