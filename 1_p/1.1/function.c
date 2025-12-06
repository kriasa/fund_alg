#include <stdio.h>
#include "head.h"
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

// a.
status print_multiplace_num(const int num, int *arr, int *count)
{
    if (num <= 0)
    {
        return ERROR_INVALID_ARGUMENT;
    }
    *count = 0;
    for (int i = num; i <= 100; i += num)
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
status hex_system(const int num, char *hex_str, int buffer_size)
{
    if (buffer_size < 3) { 
        return ERROR_OVERFLOW;
    }
    
    if (num == 0)
    {
        strcpy(hex_str, "0");
        return SUCCESS;
    }
    
    unsigned int unsigned_num;
    int is_negative = 0;
    
    if (num < 0)
    {
        is_negative = 1;
        unsigned_num = (unsigned int)(-num);
    }
    else
    {
        unsigned_num = (unsigned int)num;
    }
    
    char temp_buffer[32]; 
    char hex_chars[] = "0123456789ABCDEF";
    int i = 0;
    
    while (unsigned_num > 0)
    {
        if (i >= 31)
        {
            return ERROR_OVERFLOW;
        }
        temp_buffer[i] = hex_chars[unsigned_num % 16];
        unsigned_num /= 16;
        i++;
    }
    
    int pos = 0;
    
    if (is_negative)
    {
        if (pos >= buffer_size - 1) return ERROR_OVERFLOW;
        hex_str[pos++] = '-';
    }

    for (int j = i - 1; j >= 0; j--)
    {
        if (pos >= buffer_size - 1) return ERROR_OVERFLOW;
        hex_str[pos++] = temp_buffer[j];
        if (j > 0)
        {
            if (pos >= buffer_size - 1) return ERROR_OVERFLOW;
            hex_str[pos++] = ' ';
        }
    }
    
    hex_str[pos] = '\0';
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
        long long result = 1;
        for (int j = 1; j <= num; j++)
        {
            if (result > LLONG_MAX / i)
            {
                return ERROR_OVERFLOW;
            }
            result *= i;
            table[i - 1][j - 1] = result;
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

    if (num > 0 && (long long)num * (num + 1) / 2 > LLONG_MAX)
    {
        return ERROR_OVERFLOW;
    }
    
    *rez = (long long)num * (num + 1) / 2;
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