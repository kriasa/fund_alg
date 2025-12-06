#include "output_utils.h"
#include <stdio.h>
#include <stdbool.h>

void print_quadratic_result(const QuadraticResult *results, int count) {
    bool has_printed_solution = false;

    for (int i = 0; i < count; i++) {
        const QuadraticResult *r = &results[i];

        if (r->sol.count == -1) {

             if (r->is_unique_solution) { 
                 printf("a=%.6f, b=%.6f, c=%.6f\n", r->a, r->b, r->c);
                 printf("бесконечно много решений\n\n");
                 has_printed_solution = true;
             }
        } else if (r->sol.count == 0) {

             printf("a=%.6f, b=%.6f, c=%.6f\n", r->a, r->b, r->c);
             printf("нет корней\n\n");
             has_printed_solution = true;
        } else if (r->is_unique_solution) {
            printf("a=%.6f, b=%.6f, c=%.6f\n", r->a, r->b, r->c);
            if (r->sol.count == 1) {
                printf("один корень x = %.6f\n\n", r->sol.x1);
            } else {
                printf("два корня x1 = %.6f, x2 = %.6f\n\n", r->sol.x1, r->sol.x2);
            }
            has_printed_solution = true;
        }
    }

    if (!has_printed_solution) {
        printf("нет уникальных действительных решений\n");
    }
}

void print_multiple_result(const MultipleResult *res) {
    if (res->remainder == 0) {
        printf("%d кратно %d\n", res->a, res->b);
    } else {
        printf("%d не кратно %d. Остаток: %d\n", res->a, res->b, res->remainder);
    }
}

void print_triangle_result(const TriangleResult *res) {
    if (!res->is_triangle) {
        printf("это не треугольник\n");
    } else if (res->is_right_triangle) {
        printf("это прямоугольный треугольник (гипотенуза %.6f)\n", res->hypotenuse);
    } else {
        printf("это треугольник, но не прямоугольный\n");
    }
}

void print_usage_info(void) {
    printf("program -q epsilon a b c \n");
    printf("program -m a b\n");
    printf("program -t epsilon a b c\n");
}