#include "base_conversion.h"
#include <limits.h>

static unsigned int is_zero(unsigned int n) {
    return (n == 0) ? 1 : 0;
}

static unsigned int bitwise_less_than(unsigned int a, unsigned int b) {
    unsigned int diff = a ^ b;
    if (diff & b){
        return 1;
    }
    return 0;
}

static unsigned int bitwise_increment(unsigned int n) {
    unsigned int carry = 1;
    unsigned int bit = 1;
    
    while (carry) {
        unsigned int current_bit = n & bit;
        n = n ^ bit;
        carry = current_bit;
        bit = bit << 1;
    }
    return n;
}

static unsigned int bitwise_decrement(unsigned int n) {
    unsigned int m = 1;
    while (m) {
        unsigned int borrow = (~n) & m;
        n = n ^ m;
        if (borrow) {
            m = m << 1;
        } else {
            break;
        }
    }
    return n;
}

char digit_to_char(int digit) {
    static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
    return digits[digit & 0x1F];
}

ConversionStatus decimal_to_base2r(int decimal_num, int r, char* result, size_t result_size) {
    if (result == NULL) {
        return CONV_ERR_NULL_PTR;
    }
    
    if ((r & 0xFFFFFFF8) != 0 || r == 0) {
        return CONV_ERR_INVALID_BASE;
    }
    
    if (bitwise_less_than(result_size, 33)) {
        return CONV_ERR_BUFFER_TOO_SMALL;
    }

    int is_negative = 0;
    unsigned int num;
    
    if (decimal_num < 0) {
        is_negative = 1;
        if (decimal_num == INT_MIN) {
            num = 2147483648U;
        } else {
            num = (unsigned int)(-decimal_num);
        }
    } else {
        num = (unsigned int)decimal_num;
    }

    if (is_zero(num)) {
        result[0] = '0';
        result[1] = '\0';
        return CONV_SUCCESS;
    } 

    unsigned int index = 0;
    unsigned int base = 1U << r;
    unsigned int mask = base - 1;  
    char temp_buffer[32];
    unsigned int temp_index = 0;

    unsigned int temp_num = num;
    while (!is_zero(temp_num)) {
        if (!bitwise_less_than(temp_index, 32)) {
            return CONV_ERR_BUFFER_TOO_SMALL;
        }
        
        unsigned int digit = temp_num & mask;
        temp_buffer[temp_index] = digit_to_char((int)digit);
        temp_num = temp_num >> r;
        temp_index = bitwise_increment(temp_index);
    }
    if (is_negative) {
        result[index] = '-';
        index = bitwise_increment(index);
    }

    unsigned int reverse_index = temp_index;

    while (!is_zero(reverse_index)) {
        reverse_index = bitwise_decrement(reverse_index);
        
        if (!bitwise_less_than(index, result_size - 1)) {
            return CONV_ERR_BUFFER_TOO_SMALL;
        }
        
        result[index] = temp_buffer[reverse_index];
        index = bitwise_increment(index);
    }
    
    result[index] = '\0';
    return CONV_SUCCESS;
}