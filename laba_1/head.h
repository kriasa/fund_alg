#ifndef HEAD_H
#define HEAD_H

typedef enum
{
    SUCCESS = 0,
    ERROR_INVALID_ARGUMENT,
    ERROR_NO_MULTIPLES,
    ERROR_NO_PRIME_COMPOSITE,
    ERROR_OVERFLOW

} status;

status print_multiplace_num(const int num, int *arr, int *count);
status prime_composite_num(const int num, int *rez);
status hex_system(const int num, char *hex_str);
status print_powers_table(const int num, long long table[10][10]);
status summ_num(const int num, long long *rez);
status factorial_num(const int num, long long *rez);

#endif