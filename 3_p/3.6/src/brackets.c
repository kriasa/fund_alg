#include "../include/brackets.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *data;
    size_t capacity;
    size_t size;
} Stack;

Stack* stack_create(size_t initial_capacity) {
    if (initial_capacity == 0){ 
        return NULL;
    }
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    if (!stack){ 
        return NULL;
    }
    stack->data = (char*)malloc(initial_capacity * sizeof(char));
    if (!stack->data) {
        free(stack);
        return NULL;
    }
    
    stack->capacity = initial_capacity;
    stack->size = 0;
    return stack;
}

void stack_destroy(Stack *stack) {
    if (stack) {
        if (stack->data) {
            free(stack->data);
        }
        free(stack);
    }
}

int stack_push(Stack *stack, char value) {
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

int stack_pop(Stack *stack, char *value) {
    if (!stack || !stack->data || !value || stack->size == 0) {
        return 0;
    }
    *value = stack->data[--stack->size];
    return 1;
}

int is_bracket(char c) {
    return c == '(' || c == ')' || c == '[' || c == ']' || 
           c == '{' || c == '}' || c == '<' || c == '>';
}

int are_matching_brackets(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}') ||
           (open == '<' && close == '>');
}

BracketsStatus check_brackets(const char *str) {
    
    if (!str) {
        return BRACKETS_NULL_PTR;
    }
    
    if (str[0] == '\0') {
        return BRACKETS_EMPTY_STRING;
    }

    size_t len = strlen(str);
    if (len > MAX_INPUT_LENGTH){
        return BRACKETS_TOO_LONG;
    }
    
    Stack *stack = stack_create(16);
    if (!stack) {
        return BRACKETS_MEMORY_ERROR;
    }
    
    BracketsStatus status = BRACKETS_OK;
    
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
                status = BRACKETS_NO;
                break;
            }
            
            if (!are_matching_brackets(top, current)) {
                status = BRACKETS_NO;
                break;
            }
        }
    }
    
    if (status == BRACKETS_OK && stack->size > 0) {
        status = BRACKETS_NO;
    }
    
    stack_destroy(stack);
    return status;
}