#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "additional.h"

char* custom_strdup(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    char *new_s = malloc((len + 1) * sizeof(char));
    if (new_s == NULL) return NULL;
    strcpy(new_s, s);
    return new_s;
}

int charToVal(char c) {
    if (isdigit(c)) {
        return c - '0';
    }
    if (isalpha(c)) {
        return tolower(c) - 'a' + 10;
    }
    return -1;
}

int valToChar(int val) {
    if (val < 10) {
        return '0' + val;
    }
    return 'A' + (val - 10);
}

ReturnCode multipleString(char *numStr, int base, char **strRes) {
    int len = strlen(numStr);
    long long ost = 0;  
    int cap = len + 10;
    char *res = (char*)malloc(sizeof(char) * cap);
    if (res == NULL) {
        *strRes = NULL;
        return ERROR_MEMORY_ALLOCATION;
    }
    int ind = 0;

    for (int i = len - 1; i >= 0; i--) {
        int dig = numStr[i] - '0'; 
        long long prod = dig * base + ost;
        res[ind++] = (char)((prod % 10) + '0');
        ost = prod / 10;
    }

    while (ost > 0) {
        if (ind + 1 >= cap ) {
            cap *= 2;
            char *temp = (char*)realloc(res, cap);
            if (temp == NULL) {
                free(res);
                *strRes = NULL;
                return ERROR_MEMORY_ALLOCATION;
            }
            res = temp;
        }
        res[ind++] = (char)((ost % 10) + '0');
        ost /= 10;
    }
    res[ind] = '\0';

    for (int i = 0; i < ind / 2; i++) {
        char temp = res[i];
        res[i] = res[ind - i - 1];
        res[ind - i - 1] = temp;
    }
    *strRes = res;
    return OK;
}

ReturnCode incrementStr(char *numStr, int increm, char **strRes) {
    int len = strlen(numStr);
    long long ost = increm;
    int cap = len + 10;
    int ind = 0;
    char *res = (char*)malloc(sizeof(char) * cap);
    if (res == NULL) {
        *strRes = NULL;
        return ERROR_MEMORY_ALLOCATION;
    }
    for (int i = len - 1; i >= 0; i--) {
        int dig = numStr[i] - '0';
        long long sum = dig + ost;
        res[ind++] = (char)((sum % 10) + '0');
        ost = sum / 10;
    }
    while (ost > 0) {
        if (ind + 1 >= cap) {
            cap *= 2;
            char *temp = (char*)realloc(res, sizeof(char) * cap);
            if (temp == NULL) {
                free(res);
                *strRes = NULL;
                return ERROR_MEMORY_ALLOCATION;
            }
            res = temp;
        }
        res[ind++] = (char)((ost % 10) + '0');
        ost /= 10;
    }
    res[ind] = '\0';
    for (int i = 0; i < ind/2; i++) {
        char temp = res[i];
        res[i] = res[ind - i - 1];
        res[ind - i - 1] = temp;
    }
    *strRes = res;
    return OK;
}

ReturnCode readNumberString(Number *Num) {
    int c = 0;
    bool flag = false;
    size_t curSize = 0;
    size_t capacity = 16;

    char *buf = (char*)malloc(capacity * sizeof(char));
    if (buf == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }
    while ((c = getchar()) != EOF) {
        if (isspace(c)) {
            if (flag) {
                break;
            } else {
                continue;
            }
        }

        if (!isalnum(c) && c != '+' && c != '-'){
            while ((c = getchar()) != EOF && !isspace(c)){}; 
            free(buf);
            Num->originalNumber = NULL;
            return INVALID_NUMBER_INPUT;
        }

        if (curSize + 1 >= capacity) {
            capacity *= 2;
            char *newBuf = (char*)realloc(buf, capacity * sizeof(char));
            if (newBuf == NULL) {
                free(buf);
                Num->originalNumber = NULL;
                return ERROR_MEMORY_ALLOCATION;
            }
            buf = newBuf;
        }
        buf[curSize++] = (char)c;
        flag = true;
    }

    if (!flag) {
        free(buf);
        Num->originalNumber = NULL;
        return OK;
    }

    buf[curSize] = '\0';
    char *res = malloc(curSize + 1);
    if (res == NULL) {
        free(buf);
        Num->originalNumber = NULL;
        return ERROR_MEMORY_ALLOCATION;
    }

    strcpy(res, buf);
    Num->originalNumber = res;
    free(buf);
    return OK;
}

ReturnCode parseAndValidate(Number *num) {
    if (num == NULL || num -> originalNumber == NULL) {
        return INVALID_NUMBER_INPUT;
    }
    char *inputNum = num->originalNumber;
    if (*inputNum == '-') {
        num->znak = '-';
        inputNum++;
    } else if (*inputNum == '+'){
        num->znak = '+';
        inputNum++;
    } else {
        num->znak = '+';
    }
    while (*inputNum == '0' && *(inputNum+1) != '\0'){
        inputNum++;
    }
    if (*inputNum == '\0') {
        char *outputNum = (char*)malloc(sizeof(char) * 2);
        if (outputNum == NULL) {
            return ERROR_MEMORY_ALLOCATION;
        }
        outputNum[0] = '0';
        outputNum[1] = '\0';
        free(num->originalNumber);
        num->originalNumber = outputNum;
        num->znak = '+';
        return OK;
    }

    for (char *check = inputNum; *check; check++) {
        if (!isalnum(*check)) {
            return INVALID_NUMBER_INPUT;
        }
        if (isalpha(*check) && !isupper(*check)) {
            return INVALID_NUMBER_INPUT;  
        }
    }

    char *outputNum = custom_strdup(inputNum);  
    if (outputNum == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }

    free(num->originalNumber);
    num->originalNumber = outputNum;
    return OK;
}

ReturnCode findMinBase(Number *num) {
    int curBase = 2;
    int maxVal = 0;

    for (size_t i = 0; i < strlen(num->originalNumber); i++) {
        int val = charToVal(num->originalNumber[i]);
        if (val == -1) {
            return ERROR_INVALID_DIG; 
        }
        if (val > maxVal) {
            maxVal = val;
        }
    }
    curBase = maxVal + 1;

    if (num->base < curBase) {
        return INVALID_INPUT; 
    }
    
    if (curBase > 36) {
        return INVALID_INPUT;
    }

    return OK;
}

ReturnCode toDec(char *numStr, int base, char **outRes) {
    char *res = (char*)malloc(sizeof(char) * 2);
    if (res == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }
    res[0] = '0';
    res[1] = '\0';

    for(int i = 0; numStr[i]; i++) {
        int dig = charToVal(numStr[i]);
        if (dig < 0 || dig >= base) {
            free(res);
            *outRes = NULL;
            return ERROR_INVALID_DIG;
        }
        
        char *preResMul = NULL;
        ReturnCode status = multipleString(res, base, &preResMul);
        free(res);
        if (status != OK) {
            *outRes = NULL;
            return status;
        }
        
        char *preResAdd = NULL;
        status = incrementStr(preResMul, dig, &preResAdd);
        free(preResMul);
        if (status != OK) {
            *outRes = NULL;
            return status;
        }
        res = preResAdd;
    }
    *outRes = res;
    return OK;
}

ReturnCode moreThan(Number *num, Number *maxim) {
    if (num == NULL || maxim == NULL || num->decimalNumber == NULL) {
        return INVALID_POINTER;
    }

    size_t num_len = strlen(num->decimalNumber);
    if (maxim->decimalNumber == NULL || num_len > strlen(maxim->decimalNumber)) {
        if (maxim->decimalNumber != NULL) {
            free(maxim->decimalNumber);
            freeNum(maxim); 
        }

        maxim->decimalNumber = custom_strdup(num->decimalNumber);
        if (maxim->decimalNumber == NULL) {
            return ERROR_MEMORY_ALLOCATION;
        }
        maxim->znak = num->znak;
        return OK;
    }
    if (num_len == strlen(maxim->decimalNumber)) {
        if (strcmp(num->decimalNumber, maxim->decimalNumber) > 0) {
            freeNum(maxim); 
            
            maxim->decimalNumber = custom_strdup(num->decimalNumber);
            if (maxim->decimalNumber == NULL) {
                return ERROR_MEMORY_ALLOCATION;
            }
            maxim->znak = num->znak;
        }
    }
    return OK;
}

ReturnCode toBase(Number *num, int base, char **res) {
    if (num == NULL || num->decimalNumber == NULL || res == NULL) {
        return INVALID_POINTER;
    }
    if (base < 2 || base > 36) {
        return INVALID_INPUT;
    }

    const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int resLen = 0;
    int cap = 16;
    
    int *resDig = (int*)malloc(sizeof(int) * cap);
    if (resDig == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }

    char *decimalCopy = custom_strdup(num->decimalNumber);
    if (decimalCopy == NULL) {
        free(resDig);
        return ERROR_MEMORY_ALLOCATION;
    }

    if (strcmp(decimalCopy, "0") == 0) {
        *res = (char*)malloc(2);
        if (*res == NULL) {
            free(resDig);
            free(decimalCopy);
            return ERROR_MEMORY_ALLOCATION;
        }
        strcpy(*res, "0");
        free(resDig);
        free(decimalCopy);
        return OK;
    }

    int i = 0; 
    do {
        long long remainder = 0;
        int q_len = strlen(decimalCopy) + 1;
        char *quotient = (char*)malloc(q_len);
        if (quotient == NULL) {
            free(resDig);
            free(decimalCopy);
            return ERROR_MEMORY_ALLOCATION;
        }
        int q_idx = 0;
        bool started = false;

        for (int j = i; decimalCopy[j] != '\0'; j++) {
            int digit = decimalCopy[j] - '0';
            long long temp = remainder * 10 + digit;
            int q_val = temp / base;
            remainder = temp % base;

            if (q_val != 0 || started) {
                quotient[q_idx++] = q_val + '0';
                started = true;
            }
        }
        quotient[q_idx] = '\0';
        if (resLen >= cap) {
            cap *= 2;
            int *temp = (int*)realloc(resDig, sizeof(int) * cap);
            if (temp == NULL) {
                free(resDig);
                free(quotient);
                free(decimalCopy);
                return ERROR_MEMORY_ALLOCATION;
            }
            resDig = temp;
        }
        resDig[resLen++] = (int)remainder; 

        free(decimalCopy);

        if (q_idx == 0) {
            free(quotient);
            break; 
        }
        decimalCopy = quotient;
        i = 0; 

    } while(1);

    *res = malloc(resLen + 1 + (num->znak == '-' ? 1 : 0));
    if (!*res) {
        free(resDig);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    int current_idx = 0;
    if (num->znak == '-') {
        (*res)[current_idx++] = '-';
    }

    for (int k = 0; k < resLen; k++) {
        (*res)[current_idx++] = digits[resDig[resLen - 1 - k]];
    }
    (*res)[current_idx] = '\0';
    
    free(resDig);
    return OK;
}

void freeNum(Number *num) {
    if (num == NULL) {
        return;
    }
    if (num -> originalNumber != NULL) {
        free(num->originalNumber);
        num->originalNumber  = NULL;
    }
    if (num -> decimalNumber != NULL) {
        free(num->decimalNumber);
        num->decimalNumber = NULL;
    }
    if (num->eighteen != NULL) {
        free(num->eighteen);
        num->eighteen = NULL;
    }
    if (num -> nine != NULL) {
        free(num->nine);
        num->nine = NULL;
    }
    if (num->twentySeven != NULL) {
        free(num->twentySeven);
        num->twentySeven = NULL;
    }
    if (num ->thirtySix != NULL) {
        free(num->thirtySix);
        num->thirtySix = NULL;
    }
}

ReturnCode fullProcess(Number *num, Number *maxim) {
    ReturnCode status;

    status = parseAndValidate(num);
    if (status != OK) {
        return status;
    }

    status = findMinBase(num);
    if (status != OK) {
        return status;
    }
    
    status = toDec(num->originalNumber, num->base, &num->decimalNumber);
    if (status != OK) {
        return status;
    }

    status = moreThan(num, maxim);
    if (status != OK) {
        return status;
    }

    return OK;
}