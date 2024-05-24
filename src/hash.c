#include "hash.h"

#define BYTE_SIZE 8
#define BIT_LENGTH(num) (sizeof(num) * BYTE_SIZE)
#define BIT_MASK(bits) ((1 << bits) - 1)
#define ROTATE_RT(num, bits) ((num >> bits) | (num << (BIT_LENGTH(num) - bits)))

#define SMALL_PRIME_1 17
#define SMALL_PRIME_2 23

hash_t hash_llong(long long l)
{
    return (size_t)(l < 0 ? -l : l);
}

hash_t hash_str(const char* str)
{
    size_t code = SMALL_PRIME_1;

    while (*str)
    {
        code = (code + (size_t)*str++) * SMALL_PRIME_2;
    }

    return code;
}


hash_t hash_strn(const char* str, size_t size)
{
    size_t code = SMALL_PRIME_1;
    // const size_t words = size / sizeof(size_t);
    const size_t rest = size % sizeof(size_t);
    const size_t words_end = size - rest;

    size_t i = 0;
    for (; i < words_end; i += sizeof(size_t))
    {
        code = (code + *(size_t*)&str[i]) * SMALL_PRIME_2;
    }

    for (; i < rest; ++i)
    {
        code = (code + (size_t)str[i]) * SMALL_PRIME_2;
    }

    return code;
}


hash_t hash_double(double value)
{
    return *((size_t*)&value);
}


hash_t hash_ldouble(long double value)
{
    size_t *arr = (size_t*)&value;
    return arr[0] + (SMALL_PRIME_1 * arr[1]);
}
