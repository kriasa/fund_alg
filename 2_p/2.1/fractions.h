#ifndef FRACTIONS_H
#define FRACTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>

typedef enum {
    SUCCESS = 0,
    ERROR_MALLOC,
    ERROR_INVALID_BASE,
    ERROR_INVALID_COUNT,
    ERROR_INVALID_RANGE,
    ERROR_DIVIDE_ZERO,
    ERROR_CONVERSION_FAIL,
    ERROR_NUMBER_OUT_OF_RANGE,
    ERROR_NUMBER_TOO_CLOSE_TO_ZERO,
    ERROR_NUMBER_TOO_CLOSE_TO_ONE,
    ERROR_CONVERSION_FAILED
} error_code_t;

typedef struct {
    bool is_finite;
    error_code_t error;
    int number_index;
    double original_number;
    long long numerator;
    long long denominator;
} FractionResult;

typedef struct {
    long long p;
    long long q;
} Rational;

long long gcd(long long a, long long b);

error_code_t double_to_rational(double x, Rational *result, double epsilon);
error_code_t continued_fractions_conversion(double x, Rational *result, double epsilon);
bool check_finite_representation(long long denominator, int base, error_code_t *error);
FractionResult* define_list_of_finite_fraction_detailed(const int number_system, error_code_t *overall_error, const int n, ...);

#endif 