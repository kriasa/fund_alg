#include "file_processor.h"
#include "text_formatter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 1024
#define MAX_BUFFER_SIZE 65536

static StatusCode validate_file_path(const char* path) {
    if (path == NULL || strlen(path) == 0) {
        return ERROR_INVALID_PATH;
    }
    return SUCCESS;
}

static void free_formatted_lines(char** lines, size_t count) {
    if (lines != NULL) {
        for (size_t i = 0; i < count; i++) {
            if (lines[i] != NULL) {
                free(lines[i]);
            }
        }
        free(lines);
    }
}

static char* read_dynamic_line(FILE* file) {
    size_t buffer_size = INITIAL_BUFFER_SIZE;
    size_t length = 0;
    char* buffer = malloc(buffer_size);
    
    if (buffer == NULL) {
        return NULL;
    }
    
    while (fgets(buffer + length, buffer_size - length, file) != NULL) {
        length = strlen(buffer);
        if (length > 0 && buffer[length - 1] == '\n') {
            break;
        }
        if (length + 1 >= buffer_size) {
            if (buffer_size >= MAX_BUFFER_SIZE) {
                free(buffer);
                return NULL;
            }
            
            buffer_size *= 2;
            if (buffer_size > MAX_BUFFER_SIZE) {
                buffer_size = MAX_BUFFER_SIZE;
            }
            
            char* new_buffer = realloc(buffer, buffer_size);
            if (new_buffer == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
    }
    
    if (length == 0) {
        free(buffer);
        return NULL;
    }
    if (buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    }
    
    return buffer;
}
static char* combine_lines(const char* line1, const char* line2) {
    if (line1 == NULL && line2 == NULL) {
        return NULL;
    }
    
    size_t len1 = (line1 != NULL) ? strlen(line1) : 0;
    size_t len2 = (line2 != NULL) ? strlen(line2) : 0;
    size_t space_len = (len1 > 0 && len2 > 0) ? 1 : 0;
    
    char* combined = malloc(len1 + space_len + len2 + 1);
    if (combined == NULL) {
        return NULL;
    }
    
    combined[0] = '\0';
    
    if (line1 != NULL) {
        strcpy(combined, line1);
    }
    
    if (len1 > 0 && len2 > 0) {
        strcat(combined, " ");
    }
    
    if (line2 != NULL) {
        strcat(combined, line2);
    }
    
    return combined;
}

static StatusCode read_and_process_file(FILE* input, FILE* output) {
    char* line = NULL;
    char* remaining_text = NULL;
    
    while ((line = read_dynamic_line(input)) != NULL || remaining_text != NULL) {
        char* combined_line = NULL;
        if (remaining_text != NULL) {
            if (line != NULL) {
                combined_line = combine_lines(remaining_text, line);
                free(remaining_text);
                remaining_text = NULL;
                free(line);
                line = NULL;
            } else {
                combined_line = remaining_text;
                remaining_text = NULL;
            }
        } else if (line != NULL) {
            combined_line = line;
            line = NULL;
        }
        
        if (combined_line == NULL) {
            continue;
        }
        if (strlen(combined_line) == 0) {
            if (fputc('\n', output) == EOF) {
                free(combined_line);
                return ERROR_WRITING_FILE;
            }
            free(combined_line);
            continue;
        }
        char** formatted_lines = NULL;
        size_t line_count = 0;
        
        FormatStatus format_status = format_line_to_80_chars(combined_line, &formatted_lines, &line_count);
        
        if (format_status == FORMAT_ERROR_LONG_WORD) {
            free(combined_line);
            free_formatted_lines(formatted_lines, line_count);
            return ERROR_LONG_WORD;
        } else if (format_status != FORMAT_SUCCESS) {
            free(combined_line);
            free_formatted_lines(formatted_lines, line_count);
            return ERROR_MEMORY_ALLOCATION;
        }
        for (size_t i = 0; i < line_count; i++) {
            if (formatted_lines[i] != NULL) {
                if (fprintf(output, "%s\n", formatted_lines[i]) < 0) {
                    free(combined_line);
                    free_formatted_lines(formatted_lines, line_count);
                    return ERROR_WRITING_FILE;
                }
            }
        }
        
        free(combined_line);
        free_formatted_lines(formatted_lines, line_count);
    }
    
    if (ferror(input)) {
        return ERROR_READING_FILE;
    }
    
    return SUCCESS;
}

StatusCode process_files(const char* input_path, const char* output_path) {
    if (input_path == NULL || output_path == NULL) {
        return ERROR_NULL_POINTER;
    }
    if (strcmp(input_path, output_path) == 0) {
        return ERROR_INVALID_PATH;
    }
    
    StatusCode validation_status = validate_file_path(input_path);
    if (validation_status != SUCCESS) {
        return validation_status;
    }
    
    validation_status = validate_file_path(output_path);
    if (validation_status != SUCCESS) {
        return validation_status;
    }
    
    FILE* input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        return ERROR_OPEN_INPUT_FILE;
    }
    
    FILE* output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        fclose(input_file);
        return ERROR_OPEN_OUTPUT_FILE;
    }
    
    StatusCode process_status = read_and_process_file(input_file, output_file);
    
    fclose(input_file);
    fclose(output_file);
    
    return process_status;
}