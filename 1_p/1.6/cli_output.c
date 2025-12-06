#include "cli_output.h"
#include <stdio.h>
#include <stdlib.h>
#include "integration.h"


void print_status_message(IntegrationStatus status) {
    switch(status) {
        case INTEGRATION_SUCCESS:
            printf("Success\n");
            break;
        case INTEGRATION_INVALID_ARGUMENT:
            printf("Error b > a\n");
            break;
        case INTEGRATION_INVALID_EPSILON:
            printf("Error 0 < epsilon < 1\n");
            break;
        case INTEGRATION_MAX_ITERATIONS_EXCEEDED:
            printf("ERROR\n");
            break;
        case INTEGRATION_NULL_POINTER:
            printf("Error null pointer\n");
            break;
        case INTEGRATION_MATH_ERROR:
            printf("Error mathematical error\n");
            break;
        default:
            printf("error unknown\n");
    }
}

void run_and_print_integrals(IntegralTask integrals[], int num_integrals, double epsilon) {
    printf("epsilon = %.10g\n\n", epsilon);

    for (int i = 0; i < num_integrals; i++) {
        printf("%s\n", integrals[i].name);
        
        double result;
        IntegrationStatus status = integrate_trapezoidal(
            integrals[i].func,
            integrals[i].a,
            integrals[i].b,
            epsilon,
            &result
        );
        
        if (status == INTEGRATION_SUCCESS || status == INTEGRATION_MAX_ITERATIONS_EXCEEDED) {
            printf("%.15f\n", result);
        }
        
        print_status_message(status);
        printf("\n");
    }
}