#ifndef BRACKETS_H
#define BRACKETS_H

#include <stddef.h>
#define MAX_BRACKETS_DEPTH 10000
#define MAX_INPUT_LENGTH 1000000

typedef enum {
    BRACKETS_OK = 0,
    BRACKETS_EMPTY_STRING = 1,
    BRACKETS_NULL_PTR = 2,
    BRACKETS_MEMORY_ERROR = 3,
    BRACKETS_TOO_DEEP = 4,
    BRACKETS_TOO_LONG = 5,
    BRACKETS_NO = 6
} BracketsStatus;

BracketsStatus check_brackets(const char *str);

#endif