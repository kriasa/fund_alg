#include "integration.h"
#include <math.h>
#include <float.h>
#include <stddef.h>

#define MAX_ITERATIONS 1000000

IntegrationStatus integrate_trapezoidal(
    MathFunction f, 
    const double a, 
    const double b, 
    const double epsilon, 
    double* result) {

    if (f == NULL || result == NULL) {
        return INTEGRATION_NULL_POINTER;
    }

    if (epsilon <= 0.0 || epsilon >= 1.0) {
        return INTEGRATION_INVALID_EPSILON;
    }

    const double min_epsilon = (epsilon > DBL_EPSILON) ? epsilon : DBL_EPSILON * 1000.0;

    if (b <= a) {
        return INTEGRATION_INVALID_ARGUMENT;
    }
    
    int n = 1;
    double h = b - a;

    double I = h * 0.5 * (f(a, epsilon) + f(b, epsilon));

    if (!isfinite(I)) {
        return INTEGRATION_MATH_ERROR;
    }

    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        n *= 2;
        h /= 2.0;
        double sum_odd = 0.0;
        for (int i = 0; i < n / 2; i++) { 
            double x = a + (2 * i + 1) * h;
            double fx = f(x, epsilon);
            
            if (!isfinite(fx)) {
                return INTEGRATION_MATH_ERROR;
            }
            
            sum_odd += fx;
        }

        double newI = I * 0.5 + h * sum_odd;

        if (fabs(newI - I) < min_epsilon) {
            *result = newI;
            return INTEGRATION_SUCCESS;
        }
        
        I = newI;
    }

    *result = I;
    return INTEGRATION_MAX_ITERATIONS_EXCEEDED;
}