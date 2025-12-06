#ifndef LAB_FUNCTIONS_H
#define LAB_FUNCTIONS_H

#include <stdbool.h>

typedef enum ReturnCode {
    OK = 0,
    INVALID_INPUT,
    INVALID_FLAG,
    INVALID_NUMBER_TYPE,
    INVALID_NUMBER_LOGIC,
    NUM_OVERFLOW,
    MEMORY_ALLOCATION_ERROR,
} ReturnCode;

ReturnCode validate_and_parse_args(int argc, char *argv[], char *flag);
ReturnCode parse_float(const char *str, float *num);
ReturnCode parse_int(const char *str, int *num);

void handle_error(ReturnCode status);

typedef struct {
    float x1;
    float x2;
    int count;
} Solution;

typedef struct {
    float a, b, c;
    Solution sol;
    bool is_unique_solution;
} QuadraticResult;

typedef struct {
    int remainder;
    int a, b;
} MultipleResult;

typedef struct {
    bool is_triangle;
    bool is_right_triangle;
    float hypotenuse;
} TriangleResult;

ReturnCode process_quadratic(float *nums, QuadraticResult results[], int *results_count);
ReturnCode process_multiple(const int *nums, MultipleResult *result);
ReturnCode process_triangle(const float *nums, TriangleResult *result);

bool float_equals(const float a, const float b, const float epsilon);

Solution solve_quadratic(const float a, const float b, const float c, const float e);
void swap_float(float *a, float *b);

#endif