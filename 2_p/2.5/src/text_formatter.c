#include "text_formatter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TARGET_LENGTH 80
#define MAX_WORDS 500
#define MAX_WORD_LENGTH 100

static int is_readable_char(char c) {
    return isprint((unsigned char)c) && !isspace((unsigned char)c);
}

// Функция равномерного распределения пробелов (JUSTIFICATION)
FormatStatus distribute_spaces(char* line, size_t target_length) {
    if (line == NULL) {
        return FORMAT_ERROR_INVALID_INPUT;
    }
    
    size_t current_length = strlen(line);
    if (current_length >= target_length) {
        return FORMAT_SUCCESS;
    }
    
    // Если строка пустая или состоит из одного слова - выравнивание невозможно
    if (current_length == 0) {
        return FORMAT_SUCCESS;
    }
    
    // Считаем слова и позиции пробелов
    int space_positions[MAX_WORDS];
    char* words[MAX_WORDS];
    size_t word_count = 0;
    
    char line_copy[TARGET_LENGTH + 1];
    strcpy(line_copy, line);
    
    // Разбиваем строку на слова
    char* token = strtok(line_copy, " ");
    while (token != NULL && word_count < MAX_WORDS) {
        words[word_count] = token;
        word_count++;
        token = strtok(NULL, " ");
    }
    
    if (word_count <= 1) {
        // Одно слово - выравнивание невозможно
        return FORMAT_SUCCESS;
    }
    
    // Вычисляем сколько пробелов нужно добавить
    size_t spaces_to_add = target_length - current_length;
    size_t gaps_count = word_count - 1;
    
    if (gaps_count == 0) {
        return FORMAT_SUCCESS;
    }
    
    // Вычисляем базовое количество пробелов на каждый промежуток
    size_t base_spaces = spaces_to_add / gaps_count;
    size_t extra_spaces = spaces_to_add % gaps_count;
    
    // Строим новую строку с распределенными пробелами
    char new_line[TARGET_LENGTH + 1] = "";
    
    for (size_t i = 0; i < word_count; i++) {
        strcat(new_line, words[i]);
        
        if (i < gaps_count) {
            // Добавляем обычный пробел
            strcat(new_line, " ");
            
            // Добавляем дополнительные пробелы
            size_t extra = base_spaces + (i < extra_spaces ? 1 : 0);
            for (size_t j = 0; j < extra; j++) {
                strcat(new_line, " ");
            }
        }
    }
    
    // Копируем результат обратно
    strcpy(line, new_line);
    
    return FORMAT_SUCCESS;
}

// Функция для безопасного разбиения строки на слова
static size_t extract_words_safe(const char* text, char words[MAX_WORDS][MAX_WORD_LENGTH]) {
    if (text == NULL || *text == '\0') {
        return 0;
    }
    
    size_t word_count = 0;
    const char* current = text;
    
    while (*current != '\0' && word_count < MAX_WORDS) {
        // Пропускаем пробелы
        while (*current != '\0' && isspace((unsigned char)*current)) {
            current++;
        }
        
        if (*current == '\0') break;
        
        // Находим конец слова
        const char* word_start = current;
        while (*current != '\0' && is_readable_char(*current)) {
            current++;
        }
        
        // Копируем слово
        size_t word_len = current - word_start;
        if (word_len > 0) {
            if (word_len >= MAX_WORD_LENGTH) {
                return 0; // Слово слишком длинное
            }
            strncpy(words[word_count], word_start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

// Новая улучшенная функция форматирования
FormatStatus format_line_to_80_chars(const char* input_line, char*** formatted_lines, size_t* line_count) {
    if (input_line == NULL || formatted_lines == NULL || line_count == NULL) {
        return FORMAT_ERROR_INVALID_INPUT;
    }
    
    size_t input_length = strlen(input_line);
    
    // Проверяем на слишком длинные слова
    char words[MAX_WORDS][MAX_WORD_LENGTH];
    size_t word_count = extract_words_safe(input_line, words);
    
    if (word_count == 0) {
        // Нет слов или слишком длинное слово
        for (size_t i = 0; i < word_count; i++) {
            if (strlen(words[i]) > TARGET_LENGTH) {
                return FORMAT_ERROR_LONG_WORD;
            }
        }
        return FORMAT_ERROR_INVALID_INPUT;
    }
    
    // Если строка короткая - просто выравниваем ее
    if (input_length <= TARGET_LENGTH) {
        *formatted_lines = malloc(sizeof(char*));
        if (*formatted_lines == NULL) {
            return FORMAT_ERROR_MEMORY;
        }
        
        (*formatted_lines)[0] = malloc(TARGET_LENGTH + 1);
        if ((*formatted_lines)[0] == NULL) {
            free(*formatted_lines);
            return FORMAT_ERROR_MEMORY;
        }
        
        strcpy((*formatted_lines)[0], input_line);
        
        // ВЫРАВНИВАЕМ пробелами до 80 символов
        distribute_spaces((*formatted_lines)[0], TARGET_LENGTH);
        
        *line_count = 1;
        return FORMAT_SUCCESS;
    }
    
    // Алгоритм разбиения длинных строк
    size_t lines_needed = 1;
    size_t current_pos = 0;
    
    // Правильный алгоритм поиска точек разбиения
    while (current_pos < input_length) {
        size_t remaining = input_length - current_pos;
        
        if (remaining <= TARGET_LENGTH) {
            break;
        }
        
        // Ищем последний пробел в пределах TARGET_LENGTH
        size_t split_point = current_pos + TARGET_LENGTH;
        
        // Идем назад до пробела
        while (split_point > current_pos && !isspace((unsigned char)input_line[split_point])) {
            split_point--;
        }
        
        // Если не нашли пробел - ищем вперед до пробела
        if (split_point == current_pos) {
            split_point = current_pos + TARGET_LENGTH;
            while (split_point < input_length && !isspace((unsigned char)input_line[split_point])) {
                split_point++;
            }
            
            // Если дошли до конца и не нашли пробел - слово слишком длинное
            if (split_point == input_length && !isspace((unsigned char)input_line[split_point - 1])) {
                return FORMAT_ERROR_LONG_WORD;
            }
        }
        
        lines_needed++;
        current_pos = split_point + 1;
        
        // Пропускаем пробелы в начале следующей строки
        while (current_pos < input_length && isspace((unsigned char)input_line[current_pos])) {
            current_pos++;
        }
    }
    
    // Выделяем память для строк
    char** lines = malloc(lines_needed * sizeof(char*));
    if (lines == NULL) {
        return FORMAT_ERROR_MEMORY;
    }
    
    // Разбиваем строку и выравниваем
    current_pos = 0;
    for (size_t i = 0; i < lines_needed; i++) {
        size_t remaining = input_length - current_pos;
        
        if (remaining <= TARGET_LENGTH) {
            // Последняя строка
            lines[i] = malloc(TARGET_LENGTH + 1);
            if (lines[i] == NULL) {
                for (size_t j = 0; j < i; j++) free(lines[j]);
                free(lines);
                return FORMAT_ERROR_MEMORY;
            }
            
            strcpy(lines[i], input_line + current_pos);
            // Последнюю строку НЕ выравниваем пробелами
            *line_count = lines_needed;
            *formatted_lines = lines;
            return FORMAT_SUCCESS;
        }
        
        // Находим точку разбиения для текущей строки
        size_t split_point = current_pos + TARGET_LENGTH;
        
        // Ищем последний пробел
        while (split_point > current_pos && !isspace((unsigned char)input_line[split_point])) {
            split_point--;
        }
        
        // Если не нашли пробел - ищем вперед
        if (split_point == current_pos) {
            split_point = current_pos + TARGET_LENGTH;
            while (split_point < input_length && !isspace((unsigned char)input_line[split_point])) {
                split_point++;
            }
        }
        
        // Выделяем и копируем подстроку
        size_t line_length = split_point - current_pos;
        lines[i] = malloc(TARGET_LENGTH + 1);
        if (lines[i] == NULL) {
            for (size_t j = 0; j < i; j++) free(lines[j]);
            free(lines);
            return FORMAT_ERROR_MEMORY;
        }
        
        strncpy(lines[i], input_line + current_pos, line_length);
        lines[i][line_length] = '\0';
        
        // ВЫРАВНИВАЕМ текущую строку пробелами
        distribute_spaces(lines[i], TARGET_LENGTH);
        
        current_pos = split_point + 1;
        
        // Пропускаем пробелы
        while (current_pos < input_length && isspace((unsigned char)input_line[current_pos])) {
            current_pos++;
        }
    }
    
    *formatted_lines = lines;
    *line_count = lines_needed;
    return FORMAT_SUCCESS;
}