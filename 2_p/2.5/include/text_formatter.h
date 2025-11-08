#ifndef TEXT_FORMATTER_H
#define TEXT_FORMATTER_H

#include <stddef.h>

typedef enum {
    FORMAT_SUCCESS = 0,
    FORMAT_ERROR_MEMORY,
    FORMAT_ERROR_INVALID_INPUT,
    FORMAT_ERROR_LONG_WORD
} FormatStatus;

FormatStatus format_line_to_80_chars(const char* input_line, char*** formatted_lines, size_t* line_count);
FormatStatus distribute_spaces(char* line, size_t target_length);

#endif