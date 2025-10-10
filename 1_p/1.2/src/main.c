#include "prime_sieve.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 100

static void handle_input_error(StatusCode status, unsigned int max_limit, const char *field_name) {
    switch (status) {
        case ERROR_INVALID_INPUT:
            printf("некорректный ввод\n");
            break;
        case ERROR_OVERFLOW:
            printf("не входит в диапазон\n");
            break;
        case ERROR_UNSUPPORTED_SIZE:
            printf("%s превышает ограничение - максимум: %u\n", field_name, max_limit);
            break;
        default:
            printf("ERROR\n");
            break;
    }
}
char* read_line(void) {
    char buffer[BUFFER_SIZE];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return NULL;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    char *result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }
    strcpy(result, buffer);
    return result;
}

unsigned int read_queries_count(void) {
    unsigned int t;
    char *input;
    const char *field = "количество запросов";

    while (1) {
        printf("количество запросов: ");
        input = read_line();

        if (input == NULL) {
            printf("ошибка чтения\n");
            continue;
        }

        StatusCode status = validate_input_with_limit(input, &t, MAX_SUPPORTED_QUERIES, field);
        free(input);

        if (status == SUCCESS) {
            return t;
        } else {
            handle_input_error(status, MAX_SUPPORTED_QUERIES, field);
        }
    }
}

unsigned int read_prime_index(unsigned int query_num) {
    unsigned int n;
    char *input;
    const char *field = "порядковый номер";

    while (1) {
        printf("%u>> ", query_num);
        input = read_line();

        if (input == NULL) {
            printf("ошибка чтения\n");
            continue;
        }

        StatusCode status = validate_input_with_limit(input, &n, MAX_SUPPORTED_PRIME_INDEX, field);
        free(input);

        if (status == SUCCESS) {
            return n;
        } else {
            handle_input_error(status, MAX_SUPPORTED_PRIME_INDEX, field);
        }
    }
}

int main(void) {
    PrimeSieve sieve = {0};
    unsigned int t = read_queries_count();
    unsigned int max_query = 0;
    unsigned int *queries = (unsigned int*)malloc(t * sizeof(unsigned int));
    
    if (queries == NULL) {
        printf("ошибка выделения памяти\n");
        return EXIT_FAILURE;
    }
    for (unsigned int i = 0; i < t; i++) {
        queries[i] = read_prime_index(i + 1);
        if (queries[i] > max_query) {
            max_query = queries[i];
        }
    }

    StatusCode status = sieve_init(&sieve, max_query);
    
    if (status != SUCCESS) {
        if (status == ERROR_UNSUPPORTED_SIZE) {
            printf("запрос %u невозможен\n", max_query);
        } else if (status == ERROR_MEMORY_ALLOCATION) {
            printf("недостаточно памяти");
        } else {
            printf("ошибка инициализации решета\n");
        }
        free(queries);
        return EXIT_FAILURE;
    }
    for (unsigned int i = 0; i < t; i++) {
        unsigned int prime;
        status = sieve_get_nth_prime(&sieve, queries[i], &prime);
        
        if (status == SUCCESS) {
            printf("%u\n", prime);
        } else {
            printf("не удалось найти %u-е простое число\n", queries[i]);
        }
    }
    sieve_free(&sieve);
    free(queries);

    return EXIT_SUCCESS;
}