#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "include/base_conversion.h"

#define MAX_RESULT_SIZE 34

int main() {
    char input[100];
    int user_input;

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

        char* endptr;
        long long num = strtoll(input, &endptr, 10);

        if (endptr == input || *endptr != '\0') {
            printf("введите число или '!'\n\n");
            continue;
        }

        if (num < (long long)INT_MIN || num > (long long)INT_MAX) {
            printf("число вне диапазона int [%d; %d]\n\n", INT_MIN, INT_MAX);
            continue;
        }
        
        user_input = (int)num;
        
        char result[MAX_RESULT_SIZE];
        
        for (int r = 1; r <= 5; r++) {
            ConversionStatus status = decimal_to_base2r(user_input, r, result, sizeof(result));
            int base = 1 << r;

            switch (status) {
                case CONV_SUCCESS:
                    printf("2^%d (%2d): %s\n", r, base, result);
                    break;
                case CONV_ERR_NULL_PTR:
                    printf("2^%d (%2d): ERROR указатель NULL\n", r, base);
                    break;
                default:
                    printf("2^%d (%2d): ERROR", r, base);
                    break;
            }
        }
        printf("\n");
    }
    return 0;
}