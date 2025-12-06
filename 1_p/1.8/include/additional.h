#ifndef ADDITIONAL_H
#define ADDITIONAL_H
#include <stdio.h>

typedef enum {
    OK,
    INVALID_INPUT,
    ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_DIG,  
    INVALID_NUMBER_INPUT,
    INVALID_POINTER
} ReturnCode;

typedef struct {
    char *originalNumber;
    char *decimalNumber;
    char *nine;
    char *eighteen;
    char *twentySeven;
    char *thirtySix;
    int base;
    char znak;
} Number;

void handleError(ReturnCode status);

ReturnCode readNumberString(Number *Num);
ReturnCode parseAndValidate(Number *num);
ReturnCode findMinBase(Number *num);
ReturnCode toDec(char *numStr, int base, char **outRes);
ReturnCode fullProcess(Number *num, Number *maxim);
void freeNum(Number *num);
int valToChar(int val);
ReturnCode moreThan(Number *num, Number *maxim);
ReturnCode toBase(Number *num, int base, char **res);
int charToVal(char c);
ReturnCode multipleString(char *numStr, int base, char **strRes);
ReturnCode incrementStr(char *numStr, int increm, char **strRes);

#endif
