#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

typedef enum {
    SUCCESS = 0,
    ERROR_OPEN_INPUT_FILE,
    ERROR_OPEN_OUTPUT_FILE,
    ERROR_MEMORY_ALLOCATION,
    ERROR_READING_FILE,
    ERROR_WRITING_FILE,
    ERROR_INVALID_PATH,
    ERROR_NULL_POINTER,
    ERROR_LONG_WORD
} StatusCode;

StatusCode process_files(const char* input_path, const char* output_path);

#endif