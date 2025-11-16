#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/base_conversion.h"

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
            printf("введите целое число или '!' для выхода\n\n");
            continue;
        }

        if (num < -2147483648LL || num > 2147483647LL) {
            printf("число вне диапазона int [-2147483648; 2147483647]\n\n");
            continue;
        }
        
        user_input = (int)num;
        
        char result[33];
        
        for (int r = 1; r <= 5; r++) {
            ConversionStatus status = decimal_to_base2r(user_input, r, result, sizeof(result));
            int base = 1 << r;
            
            if (status == CONV_SUCCESS) {
                printf("2^%d (%2d): %s\n", r, base, result);
            } else {
                printf("2^%d (%2d): Ошибка конвертации (код: %d)\n", r, base, status);
            }
        }
        printf("\n");
    }
    return 0;
}