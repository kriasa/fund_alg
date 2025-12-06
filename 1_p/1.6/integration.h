#ifndef INTEGRATION_H
#define INTEGRATION_H

typedef enum IntegrationStatus {
    INTEGRATION_SUCCESS = 0,
    INTEGRATION_INVALID_ARGUMENT,
    INTEGRATION_INVALID_EPSILON,
    INTEGRATION_MAX_ITERATIONS_EXCEEDED,
    INTEGRATION_NULL_POINTER,
    INTEGRATION_MATH_ERROR
} IntegrationStatus;

typedef double (*MathFunction)(double x, double epsilon);

IntegrationStatus integrate_trapezoidal(
    MathFunction f, 
    const double a, 
    const double b, 
    const double epsilon, 
    double* result);

#endif