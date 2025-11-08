#ifndef OVERFSCANF_H
#define OVERFSCANF_H

#include <stdio.h>
#include <stdarg.h>
#include "error_codes.h"

int overfscanf(FILE* stream, const char* format, ...);
int oversscanf(const char* str, const char* format, ...);

#endif