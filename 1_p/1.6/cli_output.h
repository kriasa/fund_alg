#ifndef CLI_OUTPUT_H
#define CLI_OUTPUT_H

#include "integration.h"

typedef struct {
    const char* name;
    MathFunction func;
    double a;
    double b;
} IntegralTask;

void print_status_message(IntegrationStatus status);
void run_and_print_integrals(IntegralTask integrals[], int num_integrals, double epsilon);

#endif