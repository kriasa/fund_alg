#include "text_formatter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TARGET_LENGTH 80
#define MAX_WORDS 500

static int is_readable_char(char c) {
    return isprint((unsigned char)c) && !isspace((unsigned char)c);
}

static size_t extract_words(const char* text, char** words, size_t max_words) {
    if (text == NULL || words == NULL) {
        return 0;
    }
    
    size_t word_count = 0;
    const char* current = text;
    
    while (*current != '\0' && word_count < max_words) {

        while (*current != '\0' && isspace((unsigned char)*current)) {
            current++;
        }
        
        if (*current == '\0') break;

        const char* word_start = current;
        while (*current != '\0' && is_readable_char(*current)) {
            current++;
        }
        

        size_t word_len = current - word_start;
        words[word_count] = malloc(word_len + 1);
        if (words[word_count] == NULL) {

            for (size_t i = 0; i < word_count; i++) {
                free(words[i]);
            }
            return 0;
        }

        strncpy(words[word_count], word_start, word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    return word_count;
}

static void free_words(char** words, size_t word_count) {
    if (words != NULL) {
        for (size_t i = 0; i < word_count; i++) {
            if (words[i] != NULL) {
                free(words[i]);
            }
        }
    }
}

FormatStatus distribute_spaces(char* line, size_t target_length) {
    if (line == NULL) {
        return FORMAT_ERROR_INVALID_INPUT;
    }
    
    size_t current_length = strlen(line);

    if (current_length >= target_length || current_length == 0) {
        return FORMAT_SUCCESS;
    }

    char** words = malloc(MAX_WORDS * sizeof(char*));
    if (words == NULL) {
        return FORMAT_ERROR_MEMORY;
    }

    size_t word_count = extract_words(line, words, MAX_WORDS);
    if (word_count == 0) {
        free(words);
        return FORMAT_SUCCESS;
    }
    

    size_t words_total_length = 0;
    for (size_t i = 0; i < word_count; i++) {
        words_total_length += strlen(words[i]);
    }
    

    size_t total_spaces_needed = target_length - words_total_length;
    size_t gaps_count = word_count - 1;

    if (gaps_count == 0 || total_spaces_needed < gaps_count) {
        free_words(words, word_count);
        free(words);
        return FORMAT_SUCCESS;
    }

    size_t base_spaces_per_gap = total_spaces_needed / gaps_count;
    size_t extra_spaces = total_spaces_needed % gaps_count;

    char* new_line = malloc(target_length + 1);
    if (new_line == NULL) {
        free_words(words, word_count);
        free(words);
        return FORMAT_ERROR_MEMORY;
    }
    
    char* current_pos = new_line;
    
    for (size_t i = 0; i < word_count; i++) {

        char* word = words[i];
        size_t word_len = strlen(word);
        memcpy(current_pos, word, word_len);
        current_pos += word_len;
 
        if (i < gaps_count) {

            size_t spaces_to_add = base_spaces_per_gap;
            if (i < extra_spaces) {
                spaces_to_add++;
            }

            for (size_t j = 0; j < spaces_to_add; j++) {
                *current_pos = ' ';
                current_pos++;
            }
        }
    }

    *current_pos = '\0';

    strcpy(line, new_line);
    free(new_line);
    free_words(words, word_count);
    free(words);
    
    return FORMAT_SUCCESS;
}

static FormatStatus check_long_words(const char* text) {
    if (text == NULL) {
        return FORMAT_ERROR_INVALID_INPUT;
    }
    
    const char* current = text;
    
    while (*current != '\0') {

        while (*current != '\0' && isspace((unsigned char)*current)) {
            current++;
        }
        
        if (*current == '\0') break;

        const char* word_start = current;
        while (*current != '\0' && is_readable_char(*current)) {
            current++;
        }

        size_t word_len = current - word_start;
        if (word_len > TARGET_LENGTH) {
            return FORMAT_ERROR_LONG_WORD;
        }
    }
    
    return FORMAT_SUCCESS;
}

static char* my_strdup(const char* str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    char* new_str = malloc(len + 1);
    if (new_str != NULL) {
        strcpy(new_str, str);
    }
    return new_str;
}

FormatStatus format_line_to_80_chars(const char* input_line, char*** formatted_lines, size_t* line_count) {
    if (input_line == NULL || formatted_lines == NULL || line_count == NULL) {
        return FORMAT_ERROR_INVALID_INPUT;
    }
    
    size_t input_length = strlen(input_line);

    FormatStatus check_status = check_long_words(input_line);
    if (check_status != FORMAT_SUCCESS) {
        return check_status;
    }

    if (input_length <= TARGET_LENGTH) {
        *formatted_lines = malloc(sizeof(char*));
        if (*formatted_lines == NULL) {
            return FORMAT_ERROR_MEMORY;
        }

        (*formatted_lines)[0] = my_strdup(input_line);
        if ((*formatted_lines)[0] == NULL) {
            free(*formatted_lines);
            return FORMAT_ERROR_MEMORY;
        }

        *line_count = 1;
        return FORMAT_SUCCESS;
    }
    
    size_t lines_needed = 1;
    size_t current_pos = 0;
    size_t input_len = strlen(input_line);

    while (current_pos < input_len) {
        size_t remaining = input_len - current_pos;
        
        if (remaining <= TARGET_LENGTH) {
            break;
        }

        size_t split_point = current_pos + TARGET_LENGTH;
        if (split_point > input_len) split_point = input_len;

        while (split_point > current_pos && !isspace((unsigned char)input_line[split_point])) {
            split_point--;
        }

        if (split_point == current_pos) {
            split_point = current_pos + TARGET_LENGTH;
            if (split_point > input_len) split_point = input_len;
            
            while (split_point < input_len && !isspace((unsigned char)input_line[split_point])) {
                split_point++;
            }

            if (split_point == input_len && !isspace((unsigned char)input_line[split_point - 1])) {
                return FORMAT_ERROR_LONG_WORD;
            }
        }
        
        lines_needed++;
        current_pos = split_point + 1;

        while (current_pos < input_len && isspace((unsigned char)input_line[current_pos])) {
            current_pos++;
        }
    }

    char** lines = malloc(lines_needed * sizeof(char*));
    if (lines == NULL) {
        return FORMAT_ERROR_MEMORY;
    }

    current_pos = 0;
    for (size_t i = 0; i < lines_needed; i++) {
        size_t remaining = input_len - current_pos;
        
        if (remaining <= TARGET_LENGTH) {

            lines[i] = my_strdup(input_line + current_pos);
            if (lines[i] == NULL) {
                for (size_t j = 0; j < i; j++) free(lines[j]);
                free(lines);
                return FORMAT_ERROR_MEMORY;
            }
            *line_count = lines_needed;
            *formatted_lines = lines;
            return FORMAT_SUCCESS;
        }
        size_t split_point = current_pos + TARGET_LENGTH;
        if (split_point > input_len) split_point = input_len;
        while (split_point > current_pos && !isspace((unsigned char)input_line[split_point])) {
            split_point--;
        }
        if (split_point == current_pos) {
            split_point = current_pos + TARGET_LENGTH;
            if (split_point > input_len) split_point = input_len;
            
            while (split_point < input_len && !isspace((unsigned char)input_line[split_point])) {
                split_point++;
            }
        }
        size_t line_length = split_point - current_pos;
        lines[i] = malloc(TARGET_LENGTH + 1);
        if (lines[i] == NULL) {
            for (size_t j = 0; j < i; j++) free(lines[j]);
            free(lines);
            return FORMAT_ERROR_MEMORY;
        }
        
        strncpy(lines[i], input_line + current_pos, line_length);
        lines[i][line_length] = '\0';
        if (i < lines_needed - 1) {
            distribute_spaces(lines[i], TARGET_LENGTH);
        }
        
        current_pos = split_point + 1;
        while (current_pos < input_len && isspace((unsigned char)input_line[current_pos])) {
            current_pos++;
        }
    }
    
    *formatted_lines = lines;
    *line_count = lines_needed;
    return FORMAT_SUCCESS;
}