#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/brackets.h"

#define USER_INPUT_LENGTH 1000

void process_input(const char *input) {
    int result;
    BracketsStatus status = check_brackets(input, &result);
    switch (status) {
        case BRACKETS_OK:
            if (result) {
                printf("сбалансированы\n");
            } else {
                printf("не сбалансированы\n");
            }
            break;
        case BRACKETS_EMPTY_STRING:
            printf("пустая строка-сбалансирована\n");
            break;
        case BRACKETS_TOO_DEEP:
            printf("Ошибка ввода\n");
            break;
        case BRACKETS_TOO_LONG:
            printf("Ошибка ввода\n");
            break;
        case BRACKETS_MEMORY_ERROR:
            printf("не удалось выделить память\n");
            break;
        case BRACKETS_NULL_PTR:
            printf("передан нулевой указатель\n");
            break;
        default:
            printf("error\n");
            break;
    }
}

int main(void) {
    char input[USER_INPUT_LENGTH + 1];
    
    while (1) {
        printf(">> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {

            break;
        }
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        if (strcmp(input, "!") == 0) {
            break;
        }
        process_input(input);
    }
    
    return 0;
}