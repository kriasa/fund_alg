#include "fractions.h"
#include <string.h>

long long gcd(long long a, long long b) {
    if (a == LLONG_MIN || b == LLONG_MIN) {
        return 1;
    }
    
    a = llabs(a);
    b = llabs(b);

    if (a == 0 && b == 0) return 1;
    if (a == 0) return b;
    if (b == 0) return a;
    
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

error_code_t continued_fractions_conversion(double x, Rational *result, double epsilon) {
    
    if (!isfinite(x) || x <= 0.0 || x >= 1.0) {
        return ERROR_INVALID_RANGE;
    }

    long long a, h1, h2, k1, k2;
    h1 = 0; h2 = 1;
    k1 = 1; k2 = 0;
    
    double r = x;
    const int max_iterations = 20;
    
    for (int i = 0; i < max_iterations; i++) {
        a = (long long)r;
        
        long long h = a * h2 + h1;
        long long k = a * k2 + k1;
        
        if (k == 0 || h < 0 || k < 0) {
            return ERROR_CONVERSION_FAIL;
        }
        
        double approx = (double)h / (double)k;
        if (fabs(x - approx) < epsilon) {
            long long common_divisor = gcd(h, k);
            result->p = h / common_divisor;
            result->q = k / common_divisor;
            return SUCCESS;
        }
        
        h1 = h2; h2 = h;
        k1 = k2; k2 = k;
        
        if (fabs(r - a) < epsilon) break;
        r = 1.0 / (r - a);
    }
    
    return ERROR_CONVERSION_FAIL;
}

bool check_finite_representation(long long denominator, int base, error_code_t *error) {
    
    *error = SUCCESS;
    
    if (base <= 1) {
        *error = ERROR_INVALID_BASE;
        return false;
    }
    
    if (denominator <= 0) {
        *error = ERROR_DIVIDE_ZERO;
        return false;
    }
    
    if (denominator == 1) return true;

    long long simplified_denom = denominator;
    long long temp_base = base;
    
    while (simplified_denom > 1) {
        long long common_divisor = gcd(simplified_denom, temp_base);
        if (common_divisor == 1) break;
        simplified_denom /= common_divisor;
    }
    
    return (simplified_denom == 1);
}

error_code_t double_to_rational(double x, Rational *result, double epsilon) {
    
    if (!isfinite(x) || isnan(x)) {
        return ERROR_INVALID_RANGE;
    }
    if (x > 0.0 && x < epsilon * 10) {
        return ERROR_TOO_CLOSE_TO_ZERO;
    }
    if (x < 1.0 && x > 1.0 - epsilon * 10) {
        return ERROR_TOO_CLOSE_TO_ONE;
    }
    if (x <= 0.0 || x >= 1.0) {
        return ERROR_INVALID_RANGE;
    }
    
    return continued_fractions_conversion(x, result, epsilon);
}

FractionResult* define_list_of_finite_fraction_detailed(const int number_system, error_code_t *overall_error, const int n, ...) {
    *overall_error = SUCCESS;
    
    const double epsilon = 1e-10;

    if (number_system <= 1) {
        *overall_error = ERROR_INVALID_BASE;
        return NULL;
    }
    if (n <= 0) {
        *overall_error = ERROR_INVALID_COUNT;
        return NULL;
    }

    FractionResult* result_list = (FractionResult*)malloc(sizeof(FractionResult) * n);
    if (!result_list) {
        *overall_error = ERROR_MALLOC;
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        result_list[i].is_finite = false;
        result_list[i].error = SUCCESS;
        result_list[i].number_index = i;
        result_list[i].numerator = 0;
        result_list[i].denominator = 1;
    }
    
    va_list list_of_arguments;
    va_start(list_of_arguments, n);
    
    for (int i = 0; i < n; ++i) {
        double number = va_arg(list_of_arguments, double);
        result_list[i].original_number = number;

        if (number <= 0.0 || number >= 1.0) {
            result_list[i].error = ERROR_NUMBER_OUT_OF_RANGE;
            continue;
        }
        
        if (number > 0.0 && number < epsilon * 10) {
            result_list[i].error = ERROR_NUMBER_TOO_CLOSE_TO_ZERO;
            continue;
        }
        
        if (number < 1.0 && number > 1.0 - epsilon * 10) {
            result_list[i].error = ERROR_NUMBER_TOO_CLOSE_TO_ONE;
            continue;
        }
        
        Rational frac;
        error_code_t conv_status = double_to_rational(number, &frac, epsilon);
        
        if (conv_status != SUCCESS) {
            result_list[i].error = ERROR_CONVERSION_FAILED;
            continue;
        }

        result_list[i].numerator = frac.p;
        result_list[i].denominator = frac.q;

        error_code_t check_error = SUCCESS;
        bool finite_result = check_finite_representation(frac.q, number_system, &check_error);
        
        if (check_error != SUCCESS) {
            result_list[i].error = check_error;
            continue;
        }
        
        result_list[i].is_finite = finite_result;
        result_list[i].error = SUCCESS;
    }
    
    va_end(list_of_arguments);
    return result_list;
}