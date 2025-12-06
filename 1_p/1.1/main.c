#include "head.h"
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    if (argc != 3)
    {
        printf("Повторите ввод. Использование: <флаг> <число>\n");
        return 1;
    }

    char *flag = malloc(strlen(argv[1]) + 1);
    if (!flag) {
        printf("Ошибка выделения памяти\n");
        return 1;
    }
    strcpy(flag, argv[1]);

    char *number_str = malloc(strlen(argv[2]) + 1);
    if (!number_str) {
        printf("Ошибка выделения памяти\n");
        free(flag);
        return 1;
    }
    strcpy(number_str, argv[2]);

    if (flag[0] != '-' && flag[0] != '/')
    {
        printf("Ошибка: флаг должен начинаться с '-' или '/'.\n");
        free(flag);
        free(number_str);
        return 1;
    }
    
    if (flag[2] != '\0' || flag[1] == '\0')
    {
        printf("Ошибка: передан неправильный флаг.\n");
        free(flag);
        free(number_str);
        return 1;
    }
    
    if (number_str[0] == '\0')
    {
        printf("Ошибка: передано пустое значение для числа.\n");
        free(flag);
        free(number_str);
        return 1;
    }

    char *endptr;
    long num_l = strtol(number_str, &endptr, 10);
    
    if (*endptr != '\0')
    {
        printf("Ошибка ввода - передано не число\n");
        free(flag);
        free(number_str);
        return 1;
    }

    if (num_l > INT_MAX || num_l < INT_MIN)
    {
        printf("Введенное число вне допустимого диапазона\n");
        free(flag);
        free(number_str);
        return 1;
    }
    
    int num = (int)num_l;
    status st;
    
    switch (flag[1])
    {
    case 'h':
    {
        int count, arr[100];
        st = print_multiplace_num(num, arr, &count);
        if (st == SUCCESS)
        {
            printf("Числа, кратные %d в пределах 100: ", num);
            for (int i = 0; i < count; i++)
            {
                printf("%d ", arr[i]);
            }
            printf("\n");
        }
        else if (st == ERROR_OVERFLOW)
        {
            printf("Ошибка переполнения при вычислении кратных чисел\n");
        }
        else if (st == ERROR_NO_MULTIPLES)
        {
            printf("Нет кратных чисел для %d в пределах 100\n", num);
        }
        else if (st == ERROR_INVALID_ARGUMENT)
        {
            printf("Введено недопустимое число\n");
        }
        break;
    }
    case 'p':
    {
        int rez;
        st = prime_composite_num(num, &rez);
        if (st == SUCCESS)
        {
            if (rez == 1)
            {
                printf("%d - простое число\n", num);
            }
            else
            {
                printf("%d - составное число\n", num);
            }
        }
        else if (st == ERROR_NO_PRIME_COMPOSITE)
        {
            printf("Число %d не является ни простым ни составным\n", num);
        }
        break;
    }
    case 's':
    {

        char *hex_str = malloc(64);
        if (!hex_str) {
            printf("Ошибка выделения памяти\n");
            break;
        }
        
        st = hex_system(num, hex_str, 64);
        if (st == SUCCESS)
        {
            printf("%d в шестнадцатеричной системе: %s\n", num, hex_str);
        }
        else if (st == ERROR_OVERFLOW)
        {
            printf("Ошибка переполнения при вычислении шестнадцатеричного представления\n");
        }
        else if (st == ERROR_INVALID_ARGUMENT)
        {
            printf("Введено недопустимое число\n");
        }
        
        free(hex_str);
        break;
    }
    case 'e':
    {
        long long table[10][10];
        st = print_powers_table(num, table);
        if (st == SUCCESS)
        {
            printf("Таблица степеней до %d:\n", num);
            printf(" y^x  | ");
            for (int t = 1; t <= num; t++)
                printf("%10d", t);
            printf("\n------+");
            for (int k = 0; k < num; k++)
                printf("-----------");
            printf("\n");
            for (int i = 0; i < 10; i++)
            {
                printf("%5d |", i + 1);
                for (int j = 0; j < num; j++)
                    printf("%10lld", table[i][j]);
                printf("\n");
            }
        }
        else if (st == ERROR_OVERFLOW)
        {
            printf("Ошибка переполнения при вычислении таблицы степеней.\n");
        }
        else if (st == ERROR_INVALID_ARGUMENT)
        {
            printf("Введено значение, не принадлежащее диапазону от 1 до 10\n");
        }
        break;
    }
    case 'a':
    {
        long long rez;
        st = summ_num(num, &rez);
        if (st == SUCCESS)
        {
            printf("Сумма чисел от 1 до %d: %lld\n", num, rez);
        }
        else if (st == ERROR_OVERFLOW)
        {
            printf("Ошибка переполнения при подсчете\n");
        }
        else if (st == ERROR_INVALID_ARGUMENT)
        {
            printf("Введено недопустимое число\n");
        }
        break;
    }
    case 'f':
    {
        long long rez;
        st = factorial_num(num, &rez);
        if (st == SUCCESS)
        {
            printf("Факториал числа %d: %lld\n", num, rez);
        }
        else if (st == ERROR_OVERFLOW)
        {
            printf("Ошибка переполнения при подсчете\n");
        }
        else if (st == ERROR_INVALID_ARGUMENT)
        {
            printf("Введено недопустимое число\n");
        }
        break;
    }
    
    default:
        printf("Ошибка: неверный флаг '%c'\n", flag[1]);
    }
    free(flag);
    free(number_str);
    
    return 0;
}