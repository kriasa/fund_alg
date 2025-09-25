#include "head.h"
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    if (argc != 3)
    {
        printf("Повторите ввод. Использование : <флаг><число>\n ");
        return 1;
    }
    if (argv[1][0] != '-' && argv[1][0] != '/')
    {
        printf("Ошибка: флаг должен начинаться с '-' или '/'.\n");
        return 1;
    }
    if (argv[2][0] == '\0')
    {
        printf("Ошибка: передано пустое значение для числа.\n");
        return 1;
    }
    char *endptr;
    long num_l = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0')
    {
        printf("Ошибка ввода - передано не число");
        return 1;
    }
    if (num_l > INT_MAX || num_l < 0)
    {
        printf("Введеное число вне допустимого диапазона");
        return 1;
    }
    int num = (int)num_l;

    status st;
    switch (argv[1][1])
    {
    case 'h':
    {
        int count, arr[100];
        st = print_multiplace_num(num, arr, &count);
        if (st == SUCCESS)
        {
            printf("Числа, кратные %d в пределах 100 - ", num);
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
            printf("Нет кратных чисел кратных %d в пределах 100\n", num);
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
                printf("%d -простое число", num);
            }
            else
            {
                printf("%d -составное число", num);
            }
        }
        else if (st == ERROR_NO_PRIME_COMPOSITE)
        {
            printf("Число не является ни простым ни составным\n");
        }
        break;
    }
    case 's':
    {
        char hex_str[64];
        st = hex_system(num, hex_str);
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
        break;
    }
    case 'e':
    {
        long long table[10][10];
        st = print_powers_table(num, table);
        if (st == SUCCESS)
        {
            printf("Таблица степеней до %d:\n", num);
            printf(" y**x | ");
            for (int t = 1; t <= num; t++)
                printf("%10d", t);
            printf("\n------+");
            for (int t = 0; t < num; t++)
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
            printf("Ошибка переполнение при вычислении таблицы степеней.\n");
        }
        else if (st == ERROR_INVALID_ARGUMENT)
        {
            printf("Введено значение, не принадлежащее диапазоны от 1 до 10\n");
        }
        break;
    }
    case 'a':
    {
        long long rez;
        st = summ_num(num, &rez);
        if (st == SUCCESS)
        {
            printf("Сумма чисел от 1 до  %d: %lld\n", num, rez);
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
        printf("Ошибка: неверный флаг '%c'\n", argv[1][1]);
    }
    return 0;
}