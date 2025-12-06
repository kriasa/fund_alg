#include <stdio.h>
#include "additional.h"

void handleError(ReturnCode status) {
    switch (status) {
        case OK:
            break;
        case INVALID_INPUT:
            printf("неверный ввод\n");
            break;
        case ERROR_MEMORY_ALLOCATION:
            printf("ошибка выделения памяти\n");
            break;
        case ERROR_INVALID_DIG: 
            printf("некорректное число для системы счисления\n");
            break;
        case INVALID_NUMBER_INPUT:
            printf("число некорректно\n");
            break;
        case INVALID_POINTER:
            printf("числа не получены\n");
            break;
    }
}
