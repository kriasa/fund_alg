#ifndef STRING_LIB_H
#define STRING_LIB_H

#include <stddef.h>

typedef enum {
    SUCCESS = 0,
    ERROR_NULL_POINTER,
    ERROR_INVALID_SIZE,
    ERROR_NOT_FOUND,
    ERROR_OVERLAP,
    ERROR_INVALID_INPUT,
    ERROR_MEMORY_ALLOCATION,
    ERROR_UNKNOWN
} error_code_t;

void *my_memchr(const void *str, int c, size_t n, error_code_t *error);
int my_memcmp(const void *str1, const void *str2, size_t n, error_code_t *error);
void *my_memcpy(void *dest, const void *src, size_t n, error_code_t *error);
void *my_memset(void *str, int c, size_t n, error_code_t *error);
char *my_strncat(char *dest, const char *src, size_t n, error_code_t *error);
char *my_strchr(const char *str, int c, error_code_t *error);
int my_strncmp(const char *str1, const char *str2, size_t n, error_code_t *error);
char *my_strncpy(char *dest, const char *src, size_t n, error_code_t *error);
size_t my_strcspn(const char *str1, const char *str2, error_code_t *error);
char *my_strerror(int errnum, error_code_t *error);
size_t my_strlen(const char *str, error_code_t *error);
char *my_strpbrk(const char *str1, const char *str2, error_code_t *error);
char *my_strrchr(const char *str, int c, error_code_t *error);
char *my_strstr(const char *haystack, const char *needle, error_code_t *error);
char *my_strtok(char *str, const char *delim, error_code_t *error);

#endif