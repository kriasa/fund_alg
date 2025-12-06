#include "lab_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>

void handle_error(ReturnCode status) {
    switch (status) {
        case OK:
            return;
        case INVALID_INPUT:
            fprintf(stderr, "Ошибка: Некорректное количество аргументов для флага.\n");
            return;
        case INVALID_FLAG:
            fprintf(stderr, "Ошибка: Неверный или некорректно оформленный флаг. Используйте -q, -m или -t.\n");
            return;
        case INVALID_NUMBER_TYPE:
            fprintf(stderr, "Ошибка: Неверный формат числа в аргументах.\n");
            return;
        case INVALID_NUMBER_LOGIC:
            fprintf(stderr, "Ошибка: Нарушение логических требований к числам (e<=0, сторона<=0, числа M=0, e слишком мало).  \n");
            return;
        case NUM_OVERFLOW:
            fprintf(stderr, "Ошибка: Числовое переполнение (слишком большое/маленькое число).  \n");
            return;
        case MEMORY_ALLOCATION_ERROR:
            fprintf(stderr, "Критическая ошибка: Невозможно выделить память.  \n");
            return;
    }
}

ReturnCode parse_float(const char *str, float *num) {
    char *end;
    double res = strtod(str, &end); 

    if (end == str || *end != '\0') {
        return INVALID_NUMBER_TYPE;
    }
    if (res > FLT_MAX || res < -FLT_MAX) {
        return NUM_OVERFLOW;
    }
    *num = (float)res;
    return OK;
}

ReturnCode parse_int(const char *str, int *num) {
    char *end;
    long res = strtol(str, &end, 10);

    if (end == str || *end != '\0') {
        return INVALID_NUMBER_TYPE;
    }
    if (res > INT_MAX || res < INT_MIN) {
        return NUM_OVERFLOW;
    }
    *num = (int)res;
    return OK;
}

ReturnCode validate_and_parse_args(int argc, char *argv[], char *flag) {
    if (argc < 4 || argc > 6) {
        return INVALID_INPUT;
    }
    if ((argv[1][0] != '-' && argv[1][0] != '/') || strlen(argv[1]) != 2) {
        return INVALID_FLAG;
    }
    *flag = argv[1][1];

    switch (*flag) {
        case 'q':
        case 't':
            if (argc != 6) return INVALID_INPUT;
            break;
        case 'm':
            if (argc != 4) return INVALID_INPUT;
            break;
        default: return INVALID_FLAG;
    }
    return OK;
}

bool float_equals(const float a, const float b, const float epsilon) {
    return fabsf(a - b) < epsilon;
}

void swap_float(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

Solution solve_quadratic(const float a, const float b, const float c, const float e) {
    Solution result = {0, 0, 0};

    if (float_equals(a, 0.0f, e) && float_equals(b, 0.0f, e) && float_equals(c, 0.0f, e)) {
        result.count = -1;
        return result;
    }

    if (float_equals(a, 0.0f, e)) {
        if (float_equals(b, 0.0f, e)) {
            result.count = 0;
        } else {
            result.count = 1;
            result.x1 = -c / b;
            result.x2 = result.x1;
        }
        return result;
    }

    float discriminant = b * b - 4 * a * c;

    if (float_equals(discriminant, 0.0f, e)) {
        result.count = 1;
        result.x1 = -b / (2 * a);
        result.x2 = result.x1;
    } else if (discriminant < 0.0f) {
        result.count = 0;
    } else {
        result.count = 2;
        float sqrt_d = sqrtf(discriminant);
        result.x1 = (-b + sqrt_d) / (2 * a);
        result.x2 = (-b - sqrt_d) / (2 * a);

        if (result.x1 < result.x2) swap_float(&result.x1, &result.x2);
    }

    return result;
}

void generate_permutations_recursive(float arr[], int start, int end, const float e, 
                                     QuadraticResult results[], int *current_count) {
    if (start == end) {
        float a = arr[0];
        float b = arr[1];
        float c = arr[2];

        Solution s = solve_quadratic(a, b, c, e);

        int index = *current_count;
        results[index].a = a;
        results[index].b = b;
        results[index].c = c;
        results[index].sol = s;
        results[index].is_unique_solution = true; 

        if (s.count > 0) {
            for (int i = 0; i < index; i++) {
                if (!results[i].is_unique_solution) continue;
                
                Solution current = results[i].sol;
                
                if (s.count == current.count) {
                    if (s.count == 1) { 
                        if (float_equals(s.x1, current.x1, e)) {
                            results[index].is_unique_solution = false; 
                            break;
                        }
                    } else if (s.count == 2) { 
                        if (float_equals(s.x1, current.x1, e) && float_equals(s.x2, current.x2, e)) {
                            results[index].is_unique_solution = false;
                            break;
                        }
                    }
                }
            }
        }
        
        (*current_count)++;
        return;
    }

    for (int i = start; i <= end; i++) {
        bool skip = false;
        for (int j = start; j < i; j++) {
            if (float_equals(arr[j], arr[i], e)) {
                skip = true;
                break;
            }
        }
        if (skip) continue;
        
        swap_float(&arr[start], &arr[i]);
        generate_permutations_recursive(arr, start + 1, end, e, results, current_count);
        swap_float(&arr[start], &arr[i]);
    }
}

ReturnCode process_quadratic(float *nums, QuadraticResult results[], int *results_count) {
    float e = nums[0];
    float a = nums[1];
    float b = nums[2];
    float c = nums[3];

    if (e <= 0.0f || e < FLT_MIN) { 
        return INVALID_NUMBER_LOGIC;
    }

    *results_count = 0; 
    float coefficients[3] = {a, b, c};

    generate_permutations_recursive(coefficients, 0, 2, e, results, results_count);

    return OK;
}

ReturnCode process_multiple(const int *nums, MultipleResult *result) {
    int a = nums[0];
    int b = nums[1];

    if (a == 0 || b == 0) {
        return INVALID_NUMBER_LOGIC;
    }

    result->a = a;
    result->b = b;
    result->remainder = a % b;
    
    return OK;
}

ReturnCode process_triangle(const float *nums, TriangleResult *result) {
    float e = nums[0];
    float a = nums[1];
    float b = nums[2];
    float c = nums[3];

    result->is_triangle = false;
    result->is_right_triangle = false;
    result->hypotenuse = 0.0f;
    
    if (e <= 0.0f || a <= 0.0f || b <= 0.0f || c <= 0.0f) {
        return INVALID_NUMBER_LOGIC;
    }

    if (a + b <= c + e || a + c <= b + e || b + c <= a + e) {
        result->is_triangle = false;
        return OK;
    }

    result->is_triangle = true;

    if (float_equals(a * a + b * b, c * c, e)) {
        result->is_right_triangle = true;
        result->hypotenuse = c;
    } else if (float_equals(a * a + c * c, b * b, e)) {
        result->is_right_triangle = true;
        result->hypotenuse = b;
    } else if (float_equals(b * b + c * c, a * a, e)) {
        result->is_right_triangle = true;
        result->hypotenuse = a;
    }
    
    return OK;
}