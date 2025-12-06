#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include "integration.h"
#include "functions.h"
#include "cli_output.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error: Invalid number of arguments.\n");
        return EXIT_FAILURE;
    }

    char* endptr;
    double epsilon = strtod(argv[1], &endptr);

    if (endptr == argv[1] || *endptr != '\0') {
        fprintf(stderr, "Error: Invalid epsilon format. Must be a real number.\n");
        return EXIT_FAILURE;
    }

    if (epsilon <= 0.0 || epsilon >= 1.0) {
        fprintf(stderr, "Error: Epsilon must satisfy 0 < epsilon < 1.\n");
        return EXIT_FAILURE;
    }

    IntegralTask integrals[] = {
        {"ln(1+x)/x dx", integral_a, 0.0, 1.0},
        {"e^(-x²/2) dx", integral_b, 0.0, 1.0},
        {"ln(1/(1-x)) dx", integral_c_new, 0.0, 1.0}, 
        {"x^x dx", integral_d, 0.0, 1.0}
    };
    
    int num_integrals = sizeof(integrals) / sizeof(integrals[0]);
    run_and_print_integrals(integrals, num_integrals, epsilon);
    
    return EXIT_SUCCESS;
}