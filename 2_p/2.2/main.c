#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "string_lib.h"

char *read_line(void) {
    size_t capacity = 64;
    size_t len = 0;
    char *buffer = malloc(capacity);
    
    if (buffer == NULL) {
        return NULL;
    }
    
    printf("> ");
    fflush(stdout);
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (len + 1 >= capacity) {
            capacity *= 2;
            char *temp = realloc(buffer, capacity);
            if (temp == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }
        buffer[len++] = (char)c;
    }
    
    buffer[len] = '\0';
    return buffer;
}

char *read_dynamic_string(const char *prompt) {
    printf("%s", prompt);
    return read_line();
}

int read_char_or_number(const char *prompt) {
    char *input;
    int value = 0;
    
    while (1) {
        input = read_dynamic_string(prompt);
        if (input == NULL) continue;
        if (input[0] == '\0') {
            free(input);
            printf("Введите символ или число.\n");
            continue;
        }
        if (input[1] == '\0') {
            value = input[0];
            free(input);
            return value;
        }
        if (sscanf(input, "%d", &value) == 1) {
            free(input);
            return value;
        }
        
        printf("Введите символ или целое число.\n");
        free(input);
    }
}

int read_integer(const char *prompt) {
    char *input;
    int value = 0;
    while (1) {
        input = read_dynamic_string(prompt);
        if (input == NULL) continue;
        
        if (sscanf(input, "%d", &value) == 1) {
            free(input);
            return value;
        }
        printf("введите целое число.\n");
        free(input);
    }
}

size_t read_size_t(const char *prompt) {
    char *input;
    long long value;
    while (1) {
        input = read_dynamic_string(prompt);
        if (input == NULL) continue;
        
        if (sscanf(input, "%lld", &value) == 1 && value >= 0) {
            free(input);
            return (size_t)value;
        }
        printf("введите неотрицательное число.\n");
        free(input);
    }
}

void print_error(error_code_t error) {
    if (error == SUCCESS) return;
    
    const char *msg;
    switch (error) {
        case ERROR_NULL_POINTER: msg = "ERROR_NULL_POINTER: Один или несколько указателей равны NULL."; break;
        case ERROR_INVALID_SIZE: msg = "ERROR_INVALID_SIZE: Параметр размера равен нулю или некорректен."; break;
        case ERROR_NOT_FOUND: msg = "ERROR_NOT_FOUND: Искомый элемент/подстрока не найден."; break;
        case ERROR_OVERLAP: msg = "ERROR_OVERLAP: Блоки памяти источника и назначения пересекаются."; break;
        case ERROR_INVALID_INPUT: msg = "ERROR_INVALID_INPUT: Входные данные не прошли проверку."; break;
        case ERROR_MEMORY_ALLOCATION: msg = "ERROR_MEMORY_ALLOCATION: Не удалось выделить необходимую память."; break;
        case ERROR_UNKNOWN: default: msg = "ERROR_UNKNOWN: Произошла неизвестная ошибка в функции."; break;
    }
    fprintf(stderr, "Внутренняя ошибка функции (%d): %s\n", error, msg);
}

void demo_memchr(void) {
    char *str = read_dynamic_string("Введите строку:");
    if (str == NULL) return;
    
    int search_char = read_char_or_number("Введите искомый символ (символ или число):");
    size_t n = read_size_t("Введите количество байт для поиска:");
    
    error_code_t error;
    char *result = my_memchr(str, search_char, n, &error);
    
    if (error == SUCCESS) {
        if (result != NULL) {
            printf("Символ '%c' (ASCII %d) найден в позиции: %ld\n", 
                   (unsigned char)search_char, search_char, result - str);
            printf("Остаток строки с найденной позиции: \"%s\"\n", result);
        } else {
            printf("Символ '%c' не найден в первых %zu байтах\n", (unsigned char)search_char, n);
        }
    } else {
        print_error(error);
    }
    free(str);
}

void demo_memcmp(void) {
    char *str1 = read_dynamic_string("Введите первую строку:");
    if (str1 == NULL) return;
    
    char *str2 = read_dynamic_string("Введите вторую строку:");
    if (str2 == NULL) { free(str1); return; }
    
    size_t n = read_size_t("Введите количество байт для сравнения:");
    
    error_code_t error;
    int result = my_memcmp(str1, str2, n, &error);
    
    if (error == SUCCESS) {
        printf("Результат сравнения: %d\n", result);
        if (result == 0) {
            printf("Строки равны в первых %zu байтах\n", n);
        } else if (result < 0) {
            printf("Первая строка меньше второй\n");
        } else {
            printf("Первая строка больше второй\n");
        }
    } else {
        print_error(error);
    }
    free(str1);
    free(str2);
}

void demo_memcpy(void) {
    char *src = read_dynamic_string("Введите строку:");
    if (src == NULL) return;
    
    size_t n = read_size_t("Введите количество байт для копирования:");
    char *dest = malloc(n + 1);
    if (dest == NULL) {
        printf("ошибка выделения памяти\n");
        free(src);
        return;
    }
    
    error_code_t error;
    void *result = my_memcpy(dest, src, n, &error);
    
    if (error == SUCCESS && result != NULL) {
        dest[n] = '\0';
        printf("Успешно скопировано %zu байт\n", n);
        printf("Источник: \"%s\"\n", src);
        printf("Назначение: \"%s\"\n", dest);
    } else {
        print_error(error);
    }
    
    free(src);
    free(dest);
}

void demo_memset(void) {
    char *str = read_dynamic_string("Введите строку:");
    if (str == NULL) return;
    
    int fill_char = read_char_or_number("Введите символ для заполнения (символ или число):");
    size_t n = read_size_t("Введите количество байтов для заполнения:");
    
    error_code_t error;
    void *result = my_memset(str, fill_char, n, &error);
    
    if (error == SUCCESS && result != NULL) {
        printf("Успешно заполнено %zu байтов символом '%c'\n", n, (unsigned char)fill_char);
        printf("Результат: \"%s\"\n", (char*)result);
    } else {
        print_error(error);
    }
    free(str);
}

void demo_strlen(void) {
    char *str = read_dynamic_string("Введите строку:");
    if (str == NULL) return;
    
    error_code_t error;
    size_t len = my_strlen(str, &error);
    
    if (error == SUCCESS) {
        printf("Строка: \"%s\"\n", str);
        printf("Длина строки: %zu\n", len);
    } else {
        print_error(error);
    }
    free(str);
}

void demo_strerror(void) {
    int errnum = read_integer("Введите номер системной ошибки:");
    
    error_code_t error;
    char *error_msg = my_strerror(errnum, &error); 
    
    printf("Системная ошибка %d: %s\n", errnum, error_msg);
    if (error != SUCCESS) {
        print_error(error);
    }
}

void demo_strstr(void) {
    char *haystack = read_dynamic_string("Введите строку:");
    if (haystack == NULL) return;
    
    char *needle = read_dynamic_string("Введите подстроку:");
    if (needle == NULL) { free(haystack); return; }
    
    error_code_t error;
    char *result = my_strstr(haystack, needle, &error);
    
    if (error == SUCCESS) {
        if (result != NULL) {
            printf("Подстрока найдена в позиции: %ld\n", result - haystack);
            printf("Остаток строки с найденной позиции: \"%s\"\n", result);
        } else {
            printf("подстрока не найдена в строке\n");
        }
    } else {
        print_error(error);
    }
    free(haystack);
    free(needle);
}

void demo_strncpy(void) {
    char *src = read_dynamic_string("Введите строку:");
    if (src == NULL) return;
    
    size_t n = read_size_t("Введите количество символов для копирования:");
    
    char *dest = malloc(n + 1);
    if (dest == NULL) {
        printf("ошибка выделения памяти\n");
        free(src);
        return;
    }
    
    error_code_t error;
    char *result = my_strncpy(dest, src, n, &error);
    
    if (error == SUCCESS && result != NULL) {
        dest[n] = '\0';
        printf("Успешно скопировано до %zu символов\n", n);
        printf("Источник: \"%s\"\n", src);
        printf("Назначение: \"%s\"\n", dest);
    } else {
        print_error(error);
    }
    free(src);
    free(dest);
}

void demo_strchr(void) {
    char *str = read_dynamic_string("Введите строку:");
    if (str == NULL) return;
    
    int search_char = read_char_or_number("Введите искомый символ (символ или число):");
    
    error_code_t error;
    char *result = my_strchr(str, search_char, &error);
    
    if (error == SUCCESS) {
        if (result != NULL) {
            printf("Символ '%c' найден в позиции: %ld\n", (unsigned char)search_char, result - str);
            printf("Остаток строки с найденной позиции: \"%s\"\n", result);
        } else {
            printf("Символ '%c' не найден\n", (unsigned char)search_char);
        }
    } else {
        print_error(error);
    }
    free(str);
}

void demo_strrchr(void) {
    char *str = read_dynamic_string("Введите строку:");
    if (str == NULL) return;
    
    int search_char = read_char_or_number("Введите искомый символ (символ или число):");
    
    error_code_t error;
    char *result = my_strrchr(str, search_char, &error);
    
    if (error == SUCCESS) {
        if (result != NULL) {
            printf("Символ '%c' найден в последней позиции: %ld\n", (unsigned char)search_char, result - str);
            printf("Остаток строки с найденной позиции: \"%s\"\n", result);
        } else {
            printf("Символ '%c' не найден\n", (unsigned char)search_char);
        }
    } else {
        print_error(error);
    }
    free(str);
}

void demo_strncmp(void) {
    char *str1 = read_dynamic_string("Введите первую строку:");
    if (str1 == NULL) return;
    
    char *str2 = read_dynamic_string("Введите вторую строку:");
    if (str2 == NULL) { free(str1); return; }
    
    size_t n = read_size_t("Введите количество символов для сравнения:");
    
    error_code_t error;
    int result = my_strncmp(str1, str2, n, &error);
    
    if (error == SUCCESS) {
        printf("Результат сравнения: %d\n", result);
        if (result == 0) {
            printf("Строки равны в первых %zu символах\n", n);
        } else if (result < 0) {
            printf("Первая строка меньше второй\n");
        } else {
            printf("Первая строка больше второй\n");
        }
    } else {
        print_error(error);
    }
    free(str1);
    free(str2);
}

void demo_strncat(void) {
    char *dest = read_dynamic_string("Введите целевую строку:");
    if (dest == NULL) return;
    
    char *src = read_dynamic_string("Введите строку для добавления:");
    if (src == NULL) { free(dest); return; }
    
    size_t n = read_size_t("Введите количество символов для добавления:");

    size_t dest_len = strlen(dest);
    char *result_buffer = malloc(dest_len + n + 1);
    if (result_buffer == NULL) {
        printf("ошибка выделения памяти\n");
        free(dest);
        free(src);
        return;
    }

    strcpy(result_buffer, dest);
    
    error_code_t error;
    char *result = my_strncat(result_buffer, src, n, &error);
    
    if (error == SUCCESS && result != NULL) {
        printf("Успешно добавлено до %zu символов\n", n);
        printf("Целевая строка: \"%s\"\n", dest);
        printf("Источник: \"%s\"\n", src);
        printf("Результат: \"%s\"\n", result);
    } else {
        print_error(error);
    }
    
    free(dest);
    free(src);
    free(result_buffer);
}

void demo_strcspn(void) {
    char *str1 = read_dynamic_string("Введите строку:");
    if (str1 == NULL) return;
    
    char *str2 = read_dynamic_string("Введите символы для поиска:");
    if (str2 == NULL) { free(str1); return; }
    
    error_code_t error;
    size_t result = my_strcspn(str1, str2, &error);
    
    if (error == SUCCESS) {
        printf("Длина начального сегмента без символов '%s': %zu\n", str2, result);
        printf("Сегмент: \"%.*s\"\n", (int)result, str1);
    } else {
        print_error(error);
    }
    free(str1);
    free(str2);
}

void demo_strpbrk(void) {
    char *str1 = read_dynamic_string("Введите строку:");
    if (str1 == NULL) return;
    
    char *str2 = read_dynamic_string("Введите символы для поиска:");
    if (str2 == NULL) { free(str1); return; }
    
    error_code_t error;
    char *result = my_strpbrk(str1, str2, &error);
    
    if (error == SUCCESS) {
        if (result != NULL) {
            printf("Найден символ '%c' из набора '%s' в позиции: %ld\n", 
                   *result, str2, result - str1);
            printf("Остаток строки: \"%s\"\n", result);
        } else {
            printf("Ни один из символов '%s' не найден в строке\n", str2);
        }
    } else {
        print_error(error);
    }
    free(str1);
    free(str2);
}

void demo_strtok(void) {
    char *str = read_dynamic_string("Введите строку для разбиения:");
    if (str == NULL) return;
    
    char *delim = read_dynamic_string("Введите разделители:");
    if (delim == NULL) { free(str); return; }

    char *str_copy = malloc(strlen(str) + 1);
    if (str_copy == NULL) {
        printf("ошибка выделения памяти\n");
        free(str);
        free(delim);
        return;
    }
    strcpy(str_copy, str);
    
    printf("Токены:\n");
    error_code_t error;
    char *token = my_strtok(str_copy, delim, &error);
    int token_count = 0;
    
    while (token != NULL && error == SUCCESS) {
        printf("%d: \"%s\"\n", ++token_count, token);
        token = my_strtok(NULL, delim, &error);
    }
    
    if (error != SUCCESS) {
        print_error(error);
    }
    
    free(str);
    free(delim);
    free(str_copy);
}

void print_menu(void) {
    printf("1. memchr - найти символ в блоке памяти\n");
    printf("2. memcmp - сравнить два блока памяти\n");
    printf("3. memcpy - скопировать блок памяти\n");
    printf("4. memset - заполнить блок памяти\n");
    printf("5. strlen - получить длину строки\n");
    printf("6. strncpy - скопировать строку с ограничением\n");
    printf("7. strncat - объединить строки с ограничением\n");
    printf("8. strchr - найти первое вхождение символа\n");
    printf("9. strrchr - найти последнее вхождение символа\n");
    printf("10. strstr - найти подстроку в строке\n");
    printf("11. strncmp - сравнить строки с ограничением\n");
    printf("12. strcspn - найти длину сегмента без символов\n");
    printf("13. strpbrk - найти любой символ из набора\n");
    printf("14. strerror - получить сообщение об ошибке\n");
    printf("15. strtok - разбить строку на токены\n");
    printf("0. выход\n");
    printf("Выберите функцию: ");
}

int main(void) { 
    while (1) {
        print_menu();
        
        char *input = read_line();
        if (input == NULL) {
            printf("ошибка выделения памяти!\n");
            continue;
        }
        
        int choice;
        if (sscanf(input, "%d", &choice) != 1) {
            printf("введите число.\n");
            free(input);
            continue;
        }
        free(input);
        
        switch (choice) {
            case 1: demo_memchr(); break;
            case 2: demo_memcmp(); break;
            case 3: demo_memcpy(); break;
            case 4: demo_memset(); break;
            case 5: demo_strlen(); break;
            case 6: demo_strncpy(); break;
            case 7: demo_strncat(); break;
            case 8: demo_strchr(); break;
            case 9: demo_strrchr(); break;
            case 10: demo_strstr(); break;
            case 11: demo_strncmp(); break;
            case 12: demo_strcspn(); break;
            case 13: demo_strpbrk(); break;
            case 14: demo_strerror(); break;
            case 15: demo_strtok(); break;
            case 0: 
                printf("Выход из программы.\n");
                return 0;
            default: 
                printf("неверный выбор\n");
        }
    }
    return 0;
}