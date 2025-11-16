#include "../include/brackets.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *data;
    size_t capacity;
    size_t size;
} Stack;

static Stack* stack_create(size_t initial_capacity) {
    if (initial_capacity == 0) return NULL;
    
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) return NULL;
    
    stack->data = (char*)malloc(initial_capacity * sizeof(char));
    if (!stack->data) {
        free(stack);
        return NULL;
    }
    
    stack->capacity = initial_capacity;
    stack->size = 0;
    return stack;
}

static void stack_destroy(Stack *stack) {
    if (stack) {
        if (stack->data) {
            free(stack->data);
        }
        free(stack);
    }
}

static int stack_push(Stack *stack, char value) {
    if (!stack || !stack->data) return 0;
    
    if (stack->size >= stack->capacity) {
        size_t new_capacity = stack->capacity * 2;
        if (new_capacity <= stack->capacity) return 0;
        
        char *new_data = (char*)realloc(stack->data, new_capacity);
        if (!new_data) {
            return 0;
        }
        stack->data = new_data;
        stack->capacity = new_capacity;
    }
    
    stack->data[stack->size++] = value;
    return 1;
}

static int stack_pop(Stack *stack, char *value) {
    if (!stack || !stack->data || !value || stack->size == 0) {
        return 0;
    }
    *value = stack->data[--stack->size];
    return 1;
}

static int is_bracket(char c) {
    return c == '(' || c == ')' || c == '[' || c == ']' || 
           c == '{' || c == '}' || c == '<' || c == '>';
}

static int are_matching_brackets(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}') ||
           (open == '<' && close == '>');
}

BracketsStatus check_brackets(const char *str, int *result) {
    if (!str || !result) {
        return BRACKETS_NULL_PTR;
    }
    
    if (str[0] == '\0') {
        *result = 1;
        return BRACKETS_EMPTY_STRING;
    }

    size_t len = 0;
    while (str[len] != '\0') {
        len++;
        if (len > MAX_INPUT_LENGTH) {
            return BRACKETS_TOO_LONG;
        }
    }
    
    Stack *stack = stack_create(16);
    if (!stack) {
        return BRACKETS_MEMORY_ERROR;
    }
    
    BracketsStatus status = BRACKETS_OK;
    *result = 1;
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        char current = str[i];

        if (!is_bracket(current)) {
            continue;
        }

        if (current == '(' || current == '[' || current == '{' || current == '<') {
            if (stack->size >= MAX_BRACKETS_DEPTH) {
                status = BRACKETS_TOO_DEEP;
                break;
            }
            
            if (!stack_push(stack, current)) {
                status = BRACKETS_MEMORY_ERROR;
                break;
            }
        } 
        else {
            char top;
            if (!stack_pop(stack, &top)) {
                *result = 0;
                break;
            }
            
            if (!are_matching_brackets(top, current)) {
                *result = 0;
                break;
            }
        }
    }
    if (status == BRACKETS_OK && stack->size > 0) {
        *result = 0;
    }
    
    stack_destroy(stack);
    return status;
}