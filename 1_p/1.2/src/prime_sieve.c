#include "prime_sieve.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

size_t estimate_sieve_limit(size_t max_prime_index) {

    if (max_prime_index == 1){ 
        return 2;
    }
    
    double n_double = (double)max_prime_index;
    double log_n = log(n_double);
    double log_log_n = log(log_n);
    
    double estimate = n_double * (log_n + log_log_n - 1.0);
    size_t limit = (size_t)(estimate * 1.20) + 1000;

    if (limit > 200000000) {
        limit = 200000000;
    }

    return limit;
}

StatusCode sieve_init(PrimeSieve *sieve, size_t max_prime_index) {
    if (sieve == NULL || max_prime_index == 0) {
        return ERROR_INVALID_INPUT;
    }

    if (max_prime_index > MAX_SUPPORTED_PRIME_INDEX) {
        return ERROR_OVERFLOW;
    }

    size_t estimated_limit = estimate_sieve_limit(max_prime_index);
    if (estimated_limit > SIZE_MAX / sizeof(unsigned char)) {
        return ERROR_MEMORY_ALLOCATION;
    }

    sieve->sieve = (unsigned char*)calloc(estimated_limit + 1, sizeof(unsigned char));
    if (sieve->sieve == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }
    sieve->sieve_size = estimated_limit;

    sieve->sieve[0] = 1;
    sieve->sieve[1] = 1;

    for (size_t j = 4; j <= estimated_limit; j += 2) {
        sieve->sieve[j] = 1;
    }
    for (size_t i = 3; i * i <= estimated_limit; i += 2) {
        if (sieve->sieve[i] == 0) {
            for (size_t j = i * i; j <= estimated_limit; j += 2 * i) {
                sieve->sieve[j] = 1;
            }
        }
    }

    sieve->primes_count = 0;
    for (size_t i = 2; i <= estimated_limit; i++) {
        if (sieve->sieve[i] == 0) {
            sieve->primes_count++;
        }
    }

    if (sieve->primes_count < max_prime_index) {
        free(sieve->sieve);
        sieve->sieve = NULL;
        return ERROR_OVERFLOW;
    }

    sieve->primes = (unsigned int*)malloc(sieve->primes_count * sizeof(unsigned int));
    if (sieve->primes == NULL) {
        free(sieve->sieve);
        sieve->sieve = NULL;
        return ERROR_MEMORY_ALLOCATION;
    }

    size_t index = 0;
    for (size_t i = 2; i <= estimated_limit; i++) {
        if (sieve->sieve[i] == 0) {
            sieve->primes[index++] = (unsigned int)i;
        }
    }

    return SUCCESS;
}

void sieve_free(PrimeSieve *sieve) {
    if (sieve != NULL) {
        if (sieve->sieve != NULL) {
            free(sieve->sieve);
            sieve->sieve = NULL;
        }
        if (sieve->primes != NULL) {
            free(sieve->primes);
            sieve->primes = NULL;
        }
        sieve->sieve_size = 0;
        sieve->primes_count = 0;
    }
}

StatusCode sieve_get_nth_prime(const PrimeSieve *sieve, unsigned int n, unsigned int *result) {
    if (sieve == NULL || result == NULL) {
        return ERROR_INVALID_INPUT;
    }

    if (n == 0 || (size_t)n > sieve->primes_count) {
        return ERROR_OVERFLOW;
    }

    *result = sieve->primes[n - 1];
    return SUCCESS;
}
// const char const*input
StatusCode validate_input_with_limit(const char *input, unsigned int *value, 
                                   unsigned int max_value, const char *field_name) {
    if (input == NULL || value == NULL || field_name == NULL) {
        return ERROR_INVALID_INPUT;
    }

    if (strlen(input) == 0) {
        return ERROR_INVALID_INPUT;
    }

    if (input[0] == '-') {
        return ERROR_INVALID_INPUT;
    }

    char *endptr;
    unsigned long long temp = strtoull(input, &endptr, 10);
    if (endptr == input || *endptr != '\0') {
        return ERROR_INVALID_INPUT;
    }

    if (temp > UINT_MAX) {
        return ERROR_OVERFLOW;
    }

    *value = (unsigned int)temp;
    if (*value == 0) {
        return ERROR_INVALID_INPUT;
    }

    if (*value > max_value) {
        return ERROR_OVERFLOW;
    }

    return SUCCESS;
}