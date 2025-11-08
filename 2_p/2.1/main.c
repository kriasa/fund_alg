#include "fractions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
const double EPS = 1e-8;

error_code_t read_int(const char *prompt, int *value, int min_val, int max_val) {
    printf("%s", prompt);

    size_t size = 16;
    size_t len = 0;
    char *buffer = malloc(size);
    if (!buffer) return ERROR_MALLOC;

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (len + 1 >= size) {
            size *= 2;
            char *new_buf = realloc(buffer, size);
            if (!new_buf) {
                free(buffer);
                return ERROR_MALLOC;
            }
            buffer = new_buf;
        }
        buffer[len++] = (char)c;
    }
    buffer[len] = '\0';

    char *start = buffer;
    while (isspace((unsigned char)*start)) start++;
    char *end = buffer + strlen(buffer) - 1;
    while (end > start && isspace((unsigned char)*end)) *end-- = '\0';

    if (*start == '\0') {
        free(buffer);
        return ERROR_CONVERSION_FAIL;
    }
    char *endptr;
    long temp = strtol(start, &endptr, 10);
    if (*endptr != '\0') {
        free(buffer);
        return ERROR_CONVERSION_FAIL;
    }

    if (temp < min_val || temp > max_val) {
        free(buffer);
        return ERROR_INVALID_RANGE;
    }

    *value = (int)temp;
    free(buffer);
    return SUCCESS;
}

const char* get_error_description(error_code_t error) {
    switch (error) {
        case ERROR_NUMBER_OUT_OF_RANGE:
            return "вне диапазона (0;1)";
        case ERROR_NUMBER_TOO_CLOSE_TO_ZERO:
            return "слишком близко к 0";
        case ERROR_NUMBER_TOO_CLOSE_TO_ONE:
            return "слишком близко к 1";
        case ERROR_CONVERSION_FAILED:
            return "ошибка преобразования в дробь";
        default:
            return "неизвестная ошибка";
    }
}

void print_simple_results(FractionResult* results, int count, int base) {
    printf("\n");
    for (int i = 0; i < count; i++) {
        printf("%d: ", i + 1);
        
        if (results[i].error != SUCCESS) {
            if (fabs(results[i].original_number) < EPS) {
                printf("[ОШИБКА]%s\n", get_error_description(results[i].error));
            } else if (fabs(results[i].original_number - 1.0) < EPS) {
                printf("[ОШИБКА] %s\n", get_error_description(results[i].error));
            } else {
                printf("[ОШИБКА] %.10f - %s\n", results[i].original_number, get_error_description(results[i].error));
            }
        } else {
            printf("%.10f -> %lld/%lld -> ", 
                   results[i].original_number,
                   results[i].numerator,
                   results[i].denominator);
            
            if (results[i].is_finite) {
                printf("конечная\n");
            } else {
                printf("бесконечная\n");
            }
        }
    }
}

int main() {
    error_code_t overall_error = SUCCESS;
    FractionResult* results = NULL;
    int B;

    const int count = 7;
    double values[] = {
        0.0588235294117,
        0.33333333333333333,   
        1.0/3.0, 
        1.0/8.0,
        -0.1, 
        0.000000000001, 
        0.999999999999 
    };

    do {
        overall_error = read_int("Введите основание системы счисления B (целое > 1): ", &B, 2, INT_MAX);
        if (overall_error != SUCCESS) {
            printf("Ошибка: неверное основание. Попробуйте снова.\n");
        }
    } while (overall_error != SUCCESS);

    results = define_list_of_finite_fraction_detailed(
        B, &overall_error, count,
        values[0], values[1], values[2], values[3], 
        values[4], values[5], values[6]
    );

    if (overall_error == SUCCESS && results != NULL) {
        print_simple_results(results, count, B);
        free(results);
    } else {
        printf("Ошибка обработки\n");
        free(results);
    }

    return 0;
}