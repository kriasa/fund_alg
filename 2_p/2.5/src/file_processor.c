#include "file_processor.h"
#include "text_formatter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 1024
#define MAX_BUFFER_SIZE 65536  // 64KB максимум

static StatusCode validate_file_path(const char* path) {
    if (path == NULL || strlen(path) == 0) {
        return ERROR_INVALID_PATH;
    }
    
    // Проверяем что файлы разные
    if (strcmp(path, "output.txt") == 0) {
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

// Функция для чтения строки произвольной длины
static char* read_dynamic_line(FILE* file) {
    size_t buffer_size = INITIAL_BUFFER_SIZE;
    size_t length = 0;
    char* buffer = malloc(buffer_size);
    
    if (buffer == NULL) {
        return NULL;
    }
    
    while (fgets(buffer + length, buffer_size - length, file) != NULL) {
        length = strlen(buffer);
        
        // Проверяем достигли ли мы конца строки
        if (length > 0 && buffer[length - 1] == '\n') {
            break;
        }
        
        // Увеличиваем буфер если нужно
        if (length + 1 >= buffer_size) {
            if (buffer_size >= MAX_BUFFER_SIZE) {
                free(buffer);
                return NULL; // Слишком длинная строка
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
    
    // Убираем символ новой строки
    if (buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    }
    
    return buffer;
}

static StatusCode read_and_process_file(FILE* input, FILE* output) {
    char* line = NULL;
    char remaining_text[INITIAL_BUFFER_SIZE] = "";
    
    while ((line = read_dynamic_line(input)) != NULL || remaining_text[0] != '\0') {
        char combined_line[INITIAL_BUFFER_SIZE * 2] = "";
        
        // Объединяем остаток с новой строкой
        if (remaining_text[0] != '\0') {
            strcpy(combined_line, remaining_text);
            remaining_text[0] = '\0';
        }
        
        if (line != NULL) {
            if (combined_line[0] != '\0') {
                strcat(combined_line, " ");
            }
            strcat(combined_line, line);
            free(line);
        }
        
        // Пропускаем пустые строки
        if (strlen(combined_line) == 0) {
            if (fputc('\n', output) == EOF) {
                return ERROR_WRITING_FILE;
            }
            continue;
        }
        
        // Форматируем строку
        char** formatted_lines = NULL;
        size_t line_count = 0;
        
        FormatStatus format_status = format_line_to_80_chars(combined_line, &formatted_lines, &line_count);
        
        if (format_status == FORMAT_ERROR_LONG_WORD) {
            free_formatted_lines(formatted_lines, line_count);
            return ERROR_LONG_WORD;
        } else if (format_status != FORMAT_SUCCESS) {
            free_formatted_lines(formatted_lines, line_count);
            return ERROR_MEMORY_ALLOCATION;
        }
        
        // Записываем отформатированные строки
        for (size_t i = 0; i < line_count; i++) {
            if (formatted_lines[i] != NULL) {
                if (fprintf(output, "%s\n", formatted_lines[i]) < 0) {
                    free_formatted_lines(formatted_lines, line_count);
                    return ERROR_WRITING_FILE;
                }
            }
        }
        
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
    
    // Проверяем что файлы разные
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