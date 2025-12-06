#include "functions.h"
#include <math.h>
#include <float.h>

double integral_a(double x, double epsilon) {
    if (fabs(x) < epsilon) {
        return 1.0;
    }
    return log(1.0 + x) / x;
}

double integral_b(double x, double epsilon) {
    (void)epsilon;
    return exp(-x * x / 2.0);
}

double integral_c_new(double x, double epsilon) {
    if (fabs(1.0 - x) < epsilon) {
        return -log(epsilon);
    }

    return log(1.0 / (1.0 - x));
}

double integral_d(double x, double epsilon) {
    if (fabs(x) < epsilon) {
        return 1.0;
    }
    return pow(x, x);
}