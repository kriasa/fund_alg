#include <stdio.h>
#include "head.h"
#include <math.h>
#include <limits.h>

// a.
status print_multiplace_num(const int num, int *arr, int *count)
{
    if (num <= 0)
    {
        return ERROR_INVALID_ARGUMENT;
    }
    *count = 0;
    for (int i = 1; i <= 100; i++)
    {
        if (i % num == 0)
        {
            if (*count >= 100)
            {
                return ERROR_OVERFLOW;
            }
            arr[*count] = i;
            (*count)++;
        }
    }
    if (*count == 0)
    {
        return ERROR_NO_MULTIPLES;
    }
    return SUCCESS;
}

// b.
status prime_composite_num(const int num, int *rez)
{
    if (num <= 1)
    {
        return ERROR_NO_PRIME_COMPOSITE;
    }
    if (num == 2)
    {
        *rez = 1;
        return SUCCESS;
    }
    if (num % 2 == 0)
    {
        *rez = 0;
        return SUCCESS;
    }
    for (int i = 3; i * i <= num; i += 2)
    {
        if (num % i == 0)
        {
            *rez = 0;
            return SUCCESS;
        }
    }
    *rez = 1;
    return SUCCESS;
}

// c.
status hex_system(const int num, char *hex_str)
{
    if (num < 0)
    {
        return ERROR_INVALID_ARGUMENT;
    }
    if (num == 0)
    {
        hex_str[0] = '0';
        hex_str[1] = '\0';
        return SUCCESS;
    }
    int digit = num;
    char arg[32];
    char hex_chars[] = "0123456789ABCDEF";
    int i = 0;
    while (digit > 0)
    {
        if (i >= 32)
        {
            return ERROR_OVERFLOW;
        }
        arg[i] = hex_chars[digit % 16];
        digit /= 16;
        i++;
    }
    for (int j = i - 1; j >= 0; j--)
    {
        hex_str[2 * (i - j - 1)] = arg[j];
        hex_str[2 * (i - j - 1) + 1] = ' ';
    }
    hex_str[2 * i - 1] = '\0';
    return SUCCESS;
}

// d.
status print_powers_table(const int num, long long table[10][10])
{
    if (num > 10 || num <= 0)
    {
        return ERROR_INVALID_ARGUMENT;
    }
    for (int i = 1; i <= 10; i++)
    {
        long long rezalt = 1;
        for (int j = 1; j <= num; j++)
        {
            if (rezalt > LLONG_MAX / i)
            {
                return ERROR_OVERFLOW;
            }
            rezalt *= i;
            table[i - 1][j - 1] = rezalt;
        }
    }
    return SUCCESS;
}
// e.
status summ_num(const int num, long long *rez)
{
    *rez = 0;
    if (num <= 0)
    {
        return ERROR_INVALID_ARGUMENT;
    }
    long long digit = num * (1 + num);
    if (digit > LLONG_MAX)
    {
        return ERROR_OVERFLOW;
    }
    long long summa = digit / 2;
    *rez = summa;
    return SUCCESS;
}
// f.
status factorial_num(const int num, long long *rez)
{
    if (num < 0)
    {
        return ERROR_INVALID_ARGUMENT;
    }

    if (num == 0)
    {
        *rez = 1;
        return SUCCESS;
    }
    *rez = 1;
    for (int i = 1; i <= num; i++)
    {
        if (*rez > LLONG_MAX / i)
        {
            return ERROR_OVERFLOW;
        }
        *rez *= i;
    }
    return SUCCESS;
}
