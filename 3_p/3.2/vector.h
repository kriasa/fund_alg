#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>

#define VECTOR_STATUS_ENUM(NAME) \
typedef enum { \
    NAME##_SUCCESS = 0, \
    NAME##_ERROR_NULL_PTR, \
    NAME##_ERROR_MEMORY, \
    NAME##_ERROR_INDEX, \
    NAME##_ERROR_EMPTY, \
    NAME##_ERROR_INVALID_CAPACITY \
} NAME##_Status;


#define VECTOR_STRUCT(T, NAME) \
typedef struct { \
    T *data; \
    size_t size; \
    size_t capacity; \
    T (*CopyFunc)(const T); \
    void (*DeleteFunc)(const T); \
} NAME;


#define VECTOR_DEFINE(T, NAME) \
\
VECTOR_STATUS_ENUM(NAME) \
VECTOR_STRUCT(T, NAME) \
\
\
static size_t NAME##_size(const NAME *v) { \
    return v->size; \
} \
static size_t NAME##_capacity(const NAME *v) { \
    return v->capacity; \
} \
static int NAME##_empty(const NAME *v) { \
    return v->size == 0; \
} \
\
\
static NAME##_Status NAME##_create(NAME *v, size_t initial_capacity, T (*CopyFunc)(const T), void (*DeleteFunc)(const T)) { \
    if (v == NULL) return NAME##_ERROR_NULL_PTR; \
    if (initial_capacity == 0) initial_capacity = 1; \
\
    v->data = (T*)malloc(sizeof(T) * initial_capacity); \
    if (v->data == NULL) { \
        v->size = 0; \
        v->capacity = 0; \
        return NAME##_ERROR_MEMORY; \
    } \
\
    v->size = 0; \
    v->capacity = initial_capacity; \
    v->CopyFunc = CopyFunc; \
    v->DeleteFunc = DeleteFunc; \
    return NAME##_SUCCESS; \
} \
\
static NAME##_Status NAME##_delete(NAME *v) { \
    if (v == NULL) return NAME##_ERROR_NULL_PTR; \
\
    if (v->data != NULL) { \
        for (size_t i = 0; i < v->size; i++) { \
            if (v->DeleteFunc) v->DeleteFunc(v->data[i]); \
        } \
        free(v->data); \
    } \
\
    v->data = NULL; \
    v->size = 0; \
    v->capacity = 0; \
    return NAME##_SUCCESS; \
} \
\
static NAME##_Status NAME##_reserve(NAME *v, size_t new_capacity) { \
    if (v == NULL) return NAME##_ERROR_NULL_PTR; \
    if (new_capacity < v->size) return NAME##_ERROR_INVALID_CAPACITY; \
\
    if (new_capacity > v->capacity) { \
        T *temp = (T*)realloc(v->data, sizeof(T) * new_capacity); \
        if (temp == NULL) { \
            return NAME##_ERROR_MEMORY; \
        } \
        v->data = temp; \
        v->capacity = new_capacity; \
    } \
    return NAME##_SUCCESS; \
} \
\
static NAME##_Status NAME##_push_back(NAME *v, const T value) { \
    if (v == NULL) return NAME##_ERROR_NULL_PTR; \
\
    if (v->size == v->capacity) { \
        size_t new_capacity = v->capacity == 0 ? 1 : v->capacity * 2; \
        NAME##_Status status = NAME##_reserve(v, new_capacity); \
        if (status != NAME##_SUCCESS) return status; \
    } \
\
    v->data[v->size] = v->CopyFunc(value); \
    v->size++; \
    return NAME##_SUCCESS; \
} \
\
static NAME##_Status NAME##_get_at(const NAME *v, size_t index, T *result) { \
    if (v == NULL || result == NULL) return NAME##_ERROR_NULL_PTR; \
    if (index >= v->size) return NAME##_ERROR_INDEX; \
\
    *result = v->data[index]; \
    return NAME##_SUCCESS; \
} \
\
static NAME##_Status NAME##_delete_at(NAME *v, size_t index) { \
    if (v == NULL) return NAME##_ERROR_NULL_PTR; \
    if (v->size == 0) return NAME##_ERROR_EMPTY; \
    if (index >= v->size) return NAME##_ERROR_INDEX; \
\
    if (v->DeleteFunc) v->DeleteFunc(v->data[index]); \
\
    for (size_t i = index; i < v->size - 1; i++) { \
        v->data[i] = v->data[i + 1]; \
    } \
    v->size--; \
\
    return NAME##_SUCCESS; \
} \
\
static NAME##_Status NAME##_is_equal(const NAME *v1, const NAME *v2, int *result) { \
    if (v1 == NULL || v2 == NULL || result == NULL) return NAME##_ERROR_NULL_PTR; \
    *result = 0; \
\
    if (v1->size != v2->size) { \
        return NAME##_SUCCESS; \
    } \
\
    if (sizeof(T) <= sizeof(void*)) { \
        if (memcmp(v1->data, v2->data, v1->size * sizeof(T)) == 0) { \
            *result = 1; \
        } \
        return NAME##_SUCCESS; \
    } \
\
    for (size_t i = 0; i < v1->size; i++) { \
        T elem1 = v1->CopyFunc(v1->data[i]); \
        T elem2 = v2->CopyFunc(v2->data[i]); \
\
        if (elem1 != elem2) { \
            v1->DeleteFunc(elem1); \
            v2->DeleteFunc(elem2); \
            return NAME##_SUCCESS; \
        } \
        v1->DeleteFunc(elem1); \
        v2->DeleteFunc(elem2); \
    } \
\
    *result = 1; \
    return NAME##_SUCCESS; \
} \
\
static NAME##_Status NAME##_copy(NAME *dest, const NAME *src) { \
    if (dest == NULL || src == NULL) return NAME##_ERROR_NULL_PTR; \
\
    NAME##_Status status = NAME##_delete(dest); \
    if (status != NAME##_SUCCESS) return status; \
\
    status = NAME##_reserve(dest, src->capacity); \
    if (status != NAME##_SUCCESS) return status; \
\
    for (size_t i = 0; i < src->size; i++) { \
        dest->data[i] = dest->CopyFunc(src->data[i]); \
    } \
\
    dest->size = src->size; \
    dest->CopyFunc = src->CopyFunc; \
    dest->DeleteFunc = src->DeleteFunc; \
\
    return NAME##_SUCCESS; \
} \
\
static NAME##_Status NAME##_copy_new(const NAME *src, NAME **result) { \
    if (src == NULL || result == NULL) return NAME##_ERROR_NULL_PTR; \
\
    NAME *temp = (NAME*)malloc(sizeof(NAME)); \
    if (temp == NULL) return NAME##_ERROR_MEMORY; \
\
    NAME##_Status status = NAME##_create(temp, src->capacity, src->CopyFunc, src->DeleteFunc); \
    if (status != NAME##_SUCCESS) { \
        free(temp); \
        return status; \
    } \
\
    status = NAME##_copy(temp, src); \
    if (status != NAME##_SUCCESS) { \
        NAME##_delete(temp); \
        free(temp); \
        return status; \
    } \
\
    *result = temp; \
    return NAME##_SUCCESS; \
}

#endif