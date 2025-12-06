#include "lab_functions.h"
#include "output_utils.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PERMUTATIONS 6

int main(int argc, char *argv[]) {
    char flag;
    ReturnCode status = validate_and_parse_args(argc, argv, &flag);
    
    if (status != OK) {
        handle_error(status);
        if (status == INVALID_INPUT || status == INVALID_FLAG) {
            print_usage_info(); 
        }
        return status;
    }

    int num_count = argc - 2;
    
    switch (flag) {
        case 'q':
        case 't': {
            float *nums = NULL;
            ReturnCode final_status = OK; 

            nums = (float *)malloc(sizeof(float) * num_count);
            if (nums == NULL) {
                final_status = MEMORY_ALLOCATION_ERROR;
            }

            if (final_status == OK) {
                for (int i = 0; i < num_count; i++) {
                    final_status = parse_float(argv[i + 2], &nums[i]);
                    if (final_status != OK) {
                        break; 
                    }
                }
            }

            if (final_status == OK && nums[0] <= 0.0f) {
                final_status = INVALID_NUMBER_LOGIC;
            }


            if (final_status == OK) {
                if (flag == 'q') {
                    QuadraticResult results[MAX_PERMUTATIONS];
                    int results_count = 0;
                    final_status = process_quadratic(nums, results, &results_count);
                    if (final_status == OK) {
                        print_quadratic_result(results, results_count);
                    }
                } else {
                    TriangleResult result;
                    final_status = process_triangle(nums, &result);
                    if (final_status == OK) {
                        print_triangle_result(&result);
                    }
                }
            }

            if (final_status != OK) {
                handle_error(final_status);
            }
            if (nums) {
                free(nums);
            }
            return final_status;
        }

        case 'm': {
            int *nums = NULL;
            ReturnCode final_status = OK; 

            nums = (int *)malloc(sizeof(int) * num_count);
            if (nums == NULL) {
                final_status = MEMORY_ALLOCATION_ERROR;
            }

            if (final_status == OK) {
                for (int i = 0; i < num_count; i++) {
                    final_status = parse_int(argv[i + 2], &nums[i]); 
                    if (final_status != OK) {
                        break;
                    }
                }
            }
            
            if (final_status == OK) {
                MultipleResult result;
                final_status = process_multiple(nums, &result);
                if (final_status == OK) {
                    print_multiple_result(&result);
                }
            }
            if (final_status != OK) {
                handle_error(final_status);
            }
            if (nums) {
                free(nums);
            }
            return final_status;
        }

        default:
            handle_error(INVALID_FLAG);
            return INVALID_FLAG;
    }
}