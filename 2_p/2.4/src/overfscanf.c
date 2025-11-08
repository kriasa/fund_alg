#include "../include/overfscanf.h"
#include "../include/specifiers.h"
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

static void skip_whitespace(FILE* stream) {
    if (!stream) return;
   
    int c;
    while ((c = fgetc(stream)) != EOF && isspace((unsigned char)c));
   
    if (c != EOF) {
        ungetc(c, stream);
    }
}

static scanf_status_t read_token(FILE* stream, char* buffer, int buffer_size, int* const length) {
    if (!stream || !buffer || !length || buffer_size <= 0) {
        return SCANF_INVALID_FORMAT;
    }
   
    *length = 0;
   
    int c;
    while ((c = fgetc(stream)) != EOF && !isspace((unsigned char)c)) {
        if (*length < buffer_size - 1) {
            buffer[(*length)++] = (char)c;
        } else {
            return SCANF_MEMORY_ERROR;
        }
    }
   
    if (c != EOF) {
        ungetc(c, stream);
    }
   
    buffer[*length] = '\0';
    return (*length > 0) ? SCANF_SUCCESS : SCANF_NO_MATCH;
}

static void skip_whitespace_str(const char** str) {
    if (!str || !*str) return;
   
    while (**str && isspace((unsigned char)**str)) {
        (*str)++;
    }
}

static scanf_status_t read_token_str(const char* str_in, char* buffer, int buffer_size, int* const length, const char** str_ptr_out) {
    if (!str_in || !buffer || !length || !str_ptr_out || buffer_size <= 0) {
        return SCANF_INVALID_FORMAT;
    }
   
    *length = 0;
    const char* str = str_in;
   
    while (*str && !isspace((unsigned char)*str)) {
        if (*length < buffer_size - 1) {
            buffer[(*length)++] = *str;
            str++;
        } else {
            return SCANF_MEMORY_ERROR;
        }
    }
   
    buffer[*length] = '\0';
    *str_ptr_out = str;
    return (*length > 0) ? SCANF_SUCCESS : SCANF_NO_MATCH;
}

int overfscanf(FILE* stream, const char* format, ...) {
    if (!stream || !format) return -1;
   
    va_list args;
    va_start(args, format);
   
    int assigned_count = 0;
    const char* fmt_ptr = format;
   
    while (*fmt_ptr) {
        if (isspace((unsigned char)*fmt_ptr)) {
            fmt_ptr++;
            skip_whitespace(stream);
            continue;
        }
       
        if (*fmt_ptr != '%') {
            int c = fgetc(stream);
            if (c == EOF) break;
           
            if (c != *fmt_ptr) {
                ungetc(c, stream);
                break;
            }
           
            fmt_ptr++;
            continue;
        }
       
        fmt_ptr++;
        if (*fmt_ptr == '%') {
            int c = fgetc(stream);
            if (c != '%') {
                if (c != EOF) ungetc(c, stream);
                break;
            }
            fmt_ptr++;
            continue;
        }
       
        specifier_info_t spec_info = parse_specifier(&fmt_ptr);
        if (spec_info.type == SPECIFIER_NONE) {
            break;
        }
       
        skip_whitespace(stream);
       
        char buffer[BUFFER_SIZE];
        int token_length = 0;
        scanf_status_t read_status = read_token(stream, buffer, BUFFER_SIZE, &token_length);
       
        if (read_status != SCANF_SUCCESS) {
            break;
        }
       
        int consumed_chars = 0;
        scanf_status_t status = handle_specifier(spec_info, buffer, &args, &consumed_chars);
       
        if (status == SCANF_SUCCESS) {
            assigned_count++;
        } else {
            for (int i = token_length - 1; i >= 0; i--) {
                ungetc(buffer[i], stream);
            }
            break;
        }
    }
   
    va_end(args);
    return assigned_count;
}

int oversscanf(const char* str, const char* format, ...) {
    if (!str || !format) return -1;
   
    va_list args;
    va_start(args, format);
   
    int assigned_count = 0;
    const char* fmt_ptr = format;
    const char* str_ptr = str;
   
    while (*fmt_ptr) {
        if (isspace((unsigned char)*fmt_ptr)) {
            fmt_ptr++;
            skip_whitespace_str(&str_ptr);
            continue;
        }
       
        if (*str_ptr == '\0') {
            break;
        }

        if (*fmt_ptr != '%') {
            if (*str_ptr != *fmt_ptr) {
                break;
            }
           
            str_ptr++;
            fmt_ptr++;
            continue;
        }
       
        fmt_ptr++;
        if (*fmt_ptr == '%') {
            if (*str_ptr != '%') {
                break;
            }
           
            str_ptr++;
            fmt_ptr++;
            continue;
        }
       
        specifier_info_t spec_info = parse_specifier(&fmt_ptr);
        if (spec_info.type == SPECIFIER_NONE) {
            break;
        }
       
        const char* saved_str_ptr = str_ptr;
        skip_whitespace_str(&saved_str_ptr);

        char buffer[BUFFER_SIZE];
        int token_length = 0;
        const char* token_end_ptr = NULL;
        scanf_status_t read_status = read_token_str(saved_str_ptr, buffer, BUFFER_SIZE, &token_length, &token_end_ptr);
       
        if (read_status != SCANF_SUCCESS) {
            break;
        }
       
        int consumed_chars = 0;
        scanf_status_t status = handle_specifier(spec_info, buffer, &args, &consumed_chars);
       
        if (status == SCANF_SUCCESS) {
            assigned_count++;
            str_ptr = saved_str_ptr + consumed_chars;
        } else {
            str_ptr = saved_str_ptr + consumed_chars;
            break;
        }
    }
   
    va_end(args);
    return assigned_count;
}