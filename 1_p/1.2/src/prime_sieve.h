#ifndef PRIME_SIEVE_H
#define PRIME_SIEVE_H

#include <stddef.h>
#include <stdint.h>

#define MAX_SUPPORTED_PRIME_INDEX 10000000
#define MAX_SUPPORTED_QUERIES     100000

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_INPUT = 1,
    ERROR_MEMORY_ALLOCATION = 2,
    ERROR_OVERFLOW = 3,
    ERROR_UNSUPPORTED_SIZE = 4
} StatusCode;

typedef struct {
    unsigned char *sieve;
    unsigned int *primes;
    size_t sieve_size;
    size_t primes_count;
} PrimeSieve;

StatusCode sieve_init(PrimeSieve *sieve, size_t max_prime_index);
void sieve_free(PrimeSieve *sieve);
StatusCode sieve_get_nth_prime(const PrimeSieve *sieve, unsigned int n, unsigned int *result);
StatusCode validate_input_with_limit(const char *input, unsigned int *value, 
                                   unsigned int max_value, const char *field_name);

#endif