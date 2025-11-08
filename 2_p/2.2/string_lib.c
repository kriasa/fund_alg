#include "string_lib.h"
#include "error_messages.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

static int check_overlap(const void *ptr1, const void *ptr2, size_t n) {
    const char *p1 = (const char *)ptr1;
    const char *p2 = (const char *)ptr2;
    return (p1 < p2 && p1 + n > p2) || (p2 < p1 && p2 + n > p1);
}

void *my_memchr(const void *str, int c, size_t n, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (str == NULL) {
        if (error) *error = ERROR_NULL_POINTER;
        return NULL;
    }
    if (c < 0 || c > 255) {
        if (error) *error = ERROR_INVALID_INPUT;
        return NULL;
    }
    
    const unsigned char *ptr = (const unsigned char *)str;
    unsigned char uc = (unsigned char)c;
    
    for (size_t i = 0; i < n; i++) {
        if (ptr[i] == uc) {
            return (void *)(ptr + i);
        }
    }
    
    if (error) *error = ERROR_NOT_FOUND;
    return NULL;
}
void *my_memcpy(void *dest, const void *src, size_t n, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (dest == NULL || src == NULL) {
        if (error) *error = ERROR_NULL_POINTER;
        return NULL;
    }
    
    if (check_overlap(dest, src, n)) {
        if (error) *error = ERROR_OVERLAP;
        return NULL;
    }
    
    char *d = (char *)dest;
    const char *s = (const char *)src;
    
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    
    return dest;
}

int my_memcmp(const void *str1, const void *str2, size_t n, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (str1 == NULL || str2 == NULL) {
        if (error) *error = ERROR_NULL_POINTER;
        return 0;
    }
    
    if (n == 0) {
        return 0;
    }
    
    const unsigned char *p1 = (const unsigned char *)str1;
    const unsigned char *p2 = (const unsigned char *)str2;
    
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return (int)(p1[i] - p2[i]);
        }
    }
    
    return 0;
}

void *my_memset(void *str, int c, size_t n, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (str == NULL) {
        if (error) *error = ERROR_NULL_POINTER;
        return NULL;
    }
    
    unsigned char *ptr = (unsigned char *)str;
    unsigned char uc = (unsigned char)c;
    
    for (size_t i = 0; i < n; i++) {
        ptr[i] = uc;
    }
    
    return str;
}

char *my_strncat(char *dest, const char *src, size_t n, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (dest == NULL || src == NULL) {
        if (error) *error = ERROR_NULL_POINTER;
        return NULL;
    }
    char *d = dest;
    while (*d != '\0') {
        d++;
    }
    size_t i = 0;
    while (i < n && src[i] != '\0') {
        d[i] = src[i];
        i++;
    }
    d[i] = '\0';
    
    return dest;
}

char *my_strchr(const char *str, int c, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (str == NULL) {
        if (error) *error = ERROR_NULL_POINTER;
        return NULL;
    }
    
    unsigned char uc = (unsigned char)c;
    
    while (*str != '\0') {
        if (*str == uc) {
            return (char *)str;
        }
        str++;
    }
    
    if (uc == '\0') {
        return (char *)str;
    }
    
    if (error) *error = ERROR_NOT_FOUND;
    return NULL;
}

int my_strncmp(const char *str1, const char *str2, size_t n, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (str1 == NULL || str2 == NULL) {
        if (error) *error = ERROR_NULL_POINTER;
        return 0;
    }
    
    if (n == 0) {
        return 0;
    }
    
    for (size_t i = 0; i < n; i++) {
        if (str1[i] != str2[i]) {
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        }
        if (str1[i] == '\0') {
            break;
        }
    }
    
    return 0;
}

char *my_strncpy(char *dest, const char *src, size_t n, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (dest == NULL || src == NULL) {
        if (error) *error = ERROR_NULL_POINTER;
        return NULL;
    }
    
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    
    return dest;
}

size_t my_strcspn(const char *str1, const char *str2, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (str1 == NULL || str2 == NULL) {
        if (error) *error = ERROR_NULL_POINTER;
        return 0;
    }
    
    size_t count = 0;
    while (str1[count] != '\0') {
        const char *reject = str2;
        while (*reject != '\0') {
            if (str1[count] == *reject) {
                return count;
            }
            reject++;
        }
        count++;
    }
    
    return count;
}

char *my_strerror(int errnum, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (errnum < 0 || errnum > MAX_ERRNO) {
        if (error) *error = ERROR_INVALID_INPUT;
        return "Unknown error";
    }
    
    if (error_messages[errnum] == NULL) {
        if (error) *error = ERROR_NOT_FOUND;
        return "Unknown error";
    }
    
    return (char *)error_messages[errnum];
}

size_t my_strlen(const char *str, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (str == NULL) {
        if (error) *error = ERROR_NULL_POINTER;
        return 0;
    }
    
    const char *ptr = str;
    while (*ptr != '\0') {
        ptr++;
    }
    
    return ptr - str;
}
char *my_strpbrk(const char *str1, const char *str2, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (str1 == NULL || str2 == NULL) {
        if (error) *error = ERROR_NULL_POINTER;
        return NULL;
    }
    
    while (*str1 != '\0') {
        const char *accept = str2;
        while (*accept != '\0') {
            if (*str1 == *accept) {
                return (char *)str1;
            }
            accept++;
        }
        str1++;
    }
    
    if (error) *error = ERROR_NOT_FOUND;
    return NULL;
}

char *my_strrchr(const char *str, int c, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (str == NULL) {
        if (error) *error = ERROR_NULL_POINTER;
        return NULL;
    }
    
    const char *last = NULL;
    unsigned char uc = (unsigned char)c;
    
    while (*str != '\0') {
        if (*str == uc) {
            last = str;
        }
        str++;
    }
    
    if (uc == '\0') {
        return (char *)str;
    }
    
    if (last != NULL) {
        return (char *)last;
    }
    
    if (error) *error = ERROR_NOT_FOUND;
    return NULL;
}

char *my_strstr(const char *haystack, const char *needle, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (haystack == NULL || needle == NULL) {
        if (error) *error = ERROR_NULL_POINTER;
        return NULL;
    }
    
    if (needle[0] == '\0') {
        return (char *)haystack;
    }
    
    for (size_t i = 0; haystack[i] != '\0'; i++) {
        size_t j = 0;
        while (needle[j] != '\0' && haystack[i + j] == needle[j]) {
            j++;
        }
        if (needle[j] == '\0') {
            return (char *)(haystack + i);
        }
    }
    
    if (error) *error = ERROR_NOT_FOUND;
    return NULL;
}
static char *strtok_save = NULL;

char *my_strtok(char *str, const char *delim, error_code_t *error) {
    if (error) *error = SUCCESS;
    
    if (delim == NULL) {
        if (error) *error = ERROR_NULL_POINTER;
        return NULL;
    }   
    char *token_start;
    
    if (str != NULL) {
        strtok_save = str;
    } else if (strtok_save == NULL) {
        return NULL;
    }
    while (*strtok_save != '\0') {
        const char *d = delim;
        int is_delim = 0;
        while (*d != '\0') {
            if (*strtok_save == *d) {
                is_delim = 1;
                break;
            }
            d++;
        }
        if (!is_delim) {
            break;
        }
        strtok_save++;
    }
    
    if (*strtok_save == '\0') {
        strtok_save = NULL;
        return NULL;
    }
    
    token_start = strtok_save;
    while (*strtok_save != '\0') {
        const char *d = delim;
        int is_delim = 0;
        while (*d != '\0') {
            if (*strtok_save == *d) {
                is_delim = 1;
                break;
            }
            d++;
        }
        if (is_delim) {
            *strtok_save = '\0';
            strtok_save++;
            return token_start;
        }
        strtok_save++;
    }
    
    strtok_save = NULL;
    return token_start;
}