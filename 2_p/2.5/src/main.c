#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_processor.h"

void print_error_message(StatusCode status) {
    switch (status) {
        case SUCCESS:
            printf("Operation completed successfully.\n");
            break;
        case ERROR_OPEN_INPUT_FILE:
            printf("Error: Cannot open input file.\n");
            break;
        case ERROR_OPEN_OUTPUT_FILE:
            printf("Error: Cannot open output file.\n");
            break;
        case ERROR_MEMORY_ALLOCATION:
            printf("Error: Memory allocation failed.\n");
            break;
        case ERROR_READING_FILE:
            printf("Error: Reading from file failed.\n");
            break;
        case ERROR_WRITING_FILE:
            printf("Error: Writing to file failed.\n");
            break;
        case ERROR_INVALID_PATH:
            printf("Error: Input and output files must be different.\n");
            break;
        case ERROR_NULL_POINTER:
            printf("Error: Null pointer encountered.\n");
            break;
        case ERROR_LONG_WORD:
            printf("Error: Found word longer than 80 characters.\n");
            break;
        default:
            printf("Error: Unknown error occurred.\n");
            break;
    }
}

int main() {
    char input_path[256];
    char output_path[256];
    
    printf("Enter input file path: ");
    if (fgets(input_path, sizeof(input_path), stdin) == NULL) {
        printf("Error reading input path.\n");
        return EXIT_FAILURE;
    }
    
    // Убираем символ новой строки
    input_path[strcspn(input_path, "\n")] = '\0';
    
    printf("Enter output file path: ");
    if (fgets(output_path, sizeof(output_path), stdin) == NULL) {
        printf("Error reading output path.\n");
        return EXIT_FAILURE;
    }
    
    // Убираем символ новой строки
    output_path[strcspn(output_path, "\n")] = '\0';
    
    StatusCode result = process_files(input_path, output_path);
    
    if (result != SUCCESS) {
        print_error_message(result);
        return EXIT_FAILURE;
    }
    
    printf("File processing completed successfully.\n");
    return EXIT_SUCCESS;
}