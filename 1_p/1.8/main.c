#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "additional.h"

int main() {
    printf(">>(2-36) ");
    int base;
    if (scanf("%d", &base) != 1) {
        handleError(INVALID_INPUT);
        return INVALID_INPUT;
    }
    int c; while ((c = getchar()) != '\n' && c != EOF); 
    
    if (base < 2 || base > 36) {
        handleError(INVALID_INPUT);
        return INVALID_INPUT;
    }

    printf("num / 'Stop'>>\n");

    Number maxim = {NULL, NULL, NULL, NULL, NULL, NULL, base, '+'}; 
    
    while(1) {
        Number num = {NULL, NULL, NULL, NULL, NULL, NULL, base, '+'};
        ReturnCode status = readNumberString(&num);
        
        if (status != OK && status != INVALID_NUMBER_INPUT) {
            handleError(status);
            freeNum(&maxim);
            return status;
        }

        if (num.originalNumber == NULL) {
             freeNum(&num);
             break;
        }

        if (strcmp(num.originalNumber, "Stop") == 0) {
            freeNum(&num);
            break;
        }

        if (status == OK) {
            status = fullProcess(&num, &maxim);
            if (status != OK) {
                handleError(status);
            }
        } else {
             handleError(status);
        }

        freeNum(&num);
    }
    
    if (maxim.decimalNumber == NULL) {
        printf("нет корректных введённых чисел\n");
        freeNum(&maxim);
        return OK;
    }

    printf("\nмаксимальное число по модулю: %s\n", maxim.decimalNumber);

    ReturnCode status;
    
    status = toBase(&maxim, 9, &maxim.nine);
    if (status == OK) printf("9: %s\n", maxim.nine); else handleError(status);

    status = toBase(&maxim, 18, &maxim.eighteen);
    if (status == OK) printf("18: %s\n", maxim.eighteen); else handleError(status);

    status = toBase(&maxim, 27, &maxim.twentySeven);
    if (status == OK) printf("27: %s\n", maxim.twentySeven); else handleError(status);

    status = toBase(&maxim, 36, &maxim.thirtySix);
    if (status == OK) printf("36: %s\n", maxim.thirtySix); else handleError(status);

    freeNum(&maxim);
    return OK;
}
