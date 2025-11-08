#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/overfscanf.h"

#define MAX_INPUT 1024

void print_error_code(int code) {
    if (code >= 0) {
        printf("SUCCESS (count: %d)", code);
    } else {
        switch (code) {
            case SCANF_SUCCESS: printf("SUCCESS"); break;
            case SCANF_EOF: printf("EOF"); break;
            case SCANF_NO_MATCH: printf("NO_MATCH"); break;
            case SCANF_INVALID_FORMAT: printf("INVALID_FORMAT"); break;
            case SCANF_MEMORY_ERROR: printf("MEMORY_ERROR"); break;
            case ROMAN_INVALID_CHAR: printf("ROMAN_INVALID_CHAR"); break;
            case ROMAN_INVALID_SEQUENCE: printf("ROMAN_INVALID_SEQUENCE"); break;
            case ROMAN_OVERFLOW: printf("ROMAN_OVERFLOW"); break;
            case ZECK_INVALID_SEQUENCE: printf("ZECK_INVALID_SEQUENCE"); break;
            case ZECK_OVERFLOW: printf("ZECK_OVERFLOW"); break;
            case BASE_INVALID_CHAR: printf("BASE_INVALID_CHAR"); break;
            case BASE_INVALID_RADIX: printf("BASE_INVALID_RADIX"); break;
            case BASE_OVERFLOW: printf("BASE_OVERFLOW"); break;
            default: printf("UNKNOWN_ERROR(%d)", code); break;
        }
    }
}

void interactive_oversscanf() {
  
    char input[MAX_INPUT];
    char format[MAX_INPUT];
    
    while (1) {
        printf("\nВведите входную строку (! для выхода): ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "!") == 0) break;
        
        printf("Введите строку формата:");
        if (!fgets(format, sizeof(format), stdin)) break;
        format[strcspn(format, "\n")] = 0;

        printf("Введено '%s' с форматом '%s'\n", input, format);
        int needs_base = (strstr(format, "%Cv") != NULL || strstr(format, "%CV") != NULL);
        int base = 10;
        
        if (needs_base) {
            printf("Введите основание системы счисления (2-36) [10]: ");
            char base_input[10];
            if (fgets(base_input, sizeof(base_input), stdin)) {
                if (strlen(base_input) > 1) {
                    base = atoi(base_input);
                    if (base < 2 || base > 36) base = 10;
                }
            }
        }

        if (strstr(format, "%Ro") != NULL && strstr(format, "%Ro %Ro") != NULL) {
            int r1 = 0, r2 = 0;
            int result = oversscanf(input, "%Ro %Ro", &r1, &r2);
            printf("Результат: "); print_error_code(result); printf("\n");
            printf("Римские числа: %d, %d\n", r1, r2);
        }
        else if (strstr(format, "%Zr") != NULL && strstr(format, "%Zr %Zr") != NULL) {
            unsigned int z1 = 0, z2 = 0;
            int result = oversscanf(input, "%Zr %Zr", &z1, &z2);
            printf("Результат: "); print_error_code(result); printf("\n");
            printf("Цекендорф: %u, %u\n", z1, z2);
        }
        else if (strstr(format, "%Cv") != NULL && strstr(format, "%Cv %Cv") != NULL) {
            int b1 = 0, b2 = 0;
            int result = oversscanf(input, "%Cv %Cv", &b1, base, &b2, base);
            printf("Результат: "); print_error_code(result); printf("\n");
            printf("Числа в базе %d: %d, %d\n", base, b1, b2);
        }
        else if (strstr(format, "%d") != NULL && strstr(format, "%s") != NULL && strstr(format, "%Ro") != NULL) {
            int i1 = 0, r1 = 0;
            char str_val[100] = {0};
            int result = oversscanf(input, "%d %s %Ro", &i1, str_val, &r1);
            printf("Результат: "); print_error_code(result); printf("\n");
            printf("Результаты: целое=%d, строка='%s', римское=%d\n", i1, str_val, r1);
        }
        else if (strstr(format, "%Ro") != NULL && strstr(format, "%Zr") != NULL && 
                 strstr(format, "%Cv") != NULL && strstr(format, "%d") != NULL) {
            int r1 = 0, b1 = 0, d1 = 0;
            unsigned int z1 = 0;
            int result = oversscanf(input, "%Ro %Zr %Cv %d", &r1, &z1, &b1, base, &d1);
            printf("Результат: "); print_error_code(result); printf("\n");
            printf("Результаты: римское=%d, цекендорф=%u, база %d=%d, целое=%d\n", 
                   r1, z1, base, b1, d1);
        }
        else if (strstr(format, "%Ro") != NULL) {
            int r1 = 0;
            int result = oversscanf(input, format, &r1);
            printf("Результат: "); print_error_code(result); printf("\n");
            printf("Римское число: %d\n", r1);
        }
        else if (strstr(format, "%Zr") != NULL) {
            unsigned int z1 = 0;
            int result = oversscanf(input, format, &z1);
            printf("Результат: "); print_error_code(result); printf("\n");
            printf("Цекендорф: %u\n", z1);
        }
        else if (strstr(format, "%Cv") != NULL || strstr(format, "%CV") != NULL) {
            int b1 = 0;
            int result = oversscanf(input, format, &b1, base);
            printf("Результат: "); print_error_code(result); printf("\n");
            printf("Число в базе %d: %d\n", base, b1);
        }
        else {
            int i1 = 0;
            char str_val[100] = {0};
            float f1 = 0.0f;
            int result = oversscanf(input, format, &i1, str_val, &f1);
            printf("Результат: "); print_error_code(result); printf("\n");
            
            if (result > 0) {
                if (strstr(format, "%d")) printf("Целое число: %d\n", i1);
                if (strstr(format, "%s")) printf("Строка: '%s'\n", str_val);
                if (strstr(format, "%f")) printf("Дробное число: %.2f\n", f1);
            }
        }
    }
}

void interactive_overfscanf() {
    char filename[100];
    char format[MAX_INPUT];
    
    while (1) {
        printf("\nВведите имя файла (! для выхода): ");
        if (!fgets(filename, sizeof(filename), stdin)) break;
        filename[strcspn(filename, "\n")] = 0;
        
        if (strcmp(filename, "q") == 0) break;
        
        printf("Введите строку формата: ");
        if (!fgets(format, sizeof(format), stdin)) break;
        format[strcspn(format, "\n")] = 0;
        
        FILE* file = fopen(filename, "r");
        if (!file) {
            printf("Ошибка открытия файла %s\n", filename);
            continue;
        }
        
        printf("Чтение файла '%s' с форматом '%s'\n", filename, format);
        int needs_base = (strstr(format, "%Cv") != NULL || strstr(format, "%CV") != NULL);
        int base = 10;
        
        if (needs_base) {
            printf("Введите основание системы счисления (2-36) [10]: ");
            char base_input[10];
            if (fgets(base_input, sizeof(base_input), stdin)) {
                if (strlen(base_input) > 1) {
                    base = atoi(base_input);
                    if (base < 2 || base > 36) base = 10;
                }
            }
        }
        if (strstr(format, "%Ro") != NULL && strstr(format, "%Ro %Ro %Ro") != NULL) {
            int r1 = 0, r2 = 0, r3 = 0;
            int result = overfscanf(file, "%Ro %Ro %Ro", &r1, &r2, &r3);
            printf("Результат: "); print_error_code(result); printf("\n");
            if (result > 0) printf("Римские числа: %d, %d, %d\n", r1, r2, r3);
        }
        else if (strstr(format, "%Zr") != NULL && strstr(format, "%Zr %Zr %Zr") != NULL) {
            unsigned int z1 = 0, z2 = 0, z3 = 0;
            int result = overfscanf(file, "%Zr %Zr %Zr", &z1, &z2, &z3);
            printf("Результат: "); print_error_code(result); printf("\n");
            if (result > 0) printf("Цекендорф: %u, %u, %u\n", z1, z2, z3);
        }
        else if (strstr(format, "%Cv") != NULL && strstr(format, "%Cv %Cv %Cv") != NULL) {
            int b1 = 0, b2 = 0, b3 = 0;
            int result = overfscanf(file, "%Cv %Cv %Cv", &b1, base, &b2, base, &b3, base);
            printf("Результат: "); print_error_code(result); printf("\n");
            if (result > 0) printf("Числа в базе %d: %d, %d, %d\n", base, b1, b2, b3);
        }
        else if (strstr(format, "%d") != NULL && strstr(format, "%s") != NULL && 
                 strstr(format, "%f") != NULL && strstr(format, "%Ro") != NULL) {
            int i1 = 0, r1 = 0;
            char str_val[100] = {0};
            float f1 = 0.0f;
            int result = overfscanf(file, "%d %s %f %Ro", &i1, str_val, &f1, &r1);
            printf("Результат: "); print_error_code(result); printf("\n");
            if (result > 0) {
                printf("Результаты: целое=%d, строка='%s', дробное=%.2f, римское=%d\n", 
                       i1, str_val, f1, r1);
            }
        }
        else if (strstr(format, "%Ro") != NULL && strstr(format, "%Zr") != NULL && 
                 strstr(format, "%Cv") != NULL && strstr(format, "%d") != NULL) {
            int r1 = 0, b1 = 0, d1 = 0;
            unsigned int z1 = 0;
            int result = overfscanf(file, "%Ro %Zr %Cv %d", &r1, &z1, &b1, base, &d1);
            printf("Результат: "); print_error_code(result); printf("\n");
            if (result > 0) {
                printf("Результаты: римское=%d, цекендорф=%u, база %d=%d, целое=%d\n", 
                       r1, z1, base, b1, d1);
            }
        }
        else if (strstr(format, "%Ro") != NULL) {
            int r1 = 0;
            int result = overfscanf(file, format, &r1);
            printf("Результат: "); print_error_code(result); printf("\n");
            if (result > 0) printf("Римское число: %d\n", r1);
        }
        else if (strstr(format, "%Zr") != NULL) {
            unsigned int z1 = 0;
            int result = overfscanf(file, format, &z1);
            printf("Результат: "); print_error_code(result); printf("\n");
            if (result > 0) printf("Цекендорф: %u\n", z1);
        }
        else if (strstr(format, "%Cv") != NULL || strstr(format, "%CV") != NULL) {
            int b1 = 0;
            int result = overfscanf(file, format, &b1, base);
            printf("Результат: "); print_error_code(result); printf("\n");
            if (result > 0) printf("Число в базе %d: %d\n", base, b1);
        }
        else {
            int i1 = 0;
            int result = overfscanf(file, format, &i1);
            printf("Результат: "); print_error_code(result); printf("\n");
            if (result > 0) printf("Целое число: %d\n", i1);
        }
        
        fclose(file);
    }
}

int main() {  
    
    char choice;
    
    while (1) {
        printf("\n1 oversscanf (парсинг строки)\n");
        printf("2 overfscanf (парсинг файла)\n");
        printf("0 Выход\n");
        printf("Выбор: ");
        
        if (scanf(" %c", &choice) != 1) break;
        getchar();
        
        switch (choice) {
            case '1':
                interactive_oversscanf();
                break;
            case '2':
                interactive_overfscanf();
                break;
            case '0':
                return 0;
            default:
                printf("Попробуйте снова.\n");
        }
    }
    
    return 0;
}