#include "hash.h"
#include <assert.h>

#define BYTE_SIZE 8
#define BIT_LENGTH(num) (sizeof(num) * BYTE_SIZE)
#define BIT_MASK(bits) ((1 << bits) - 1)
#define ROTATE_RT(num, bits) ((num >> bits) | (num << (BIT_LENGTH(num) - bits)))

#define SMALL_PRIME_1 17
#define SMALL_PRIME_2 23



hash_t hash_ptr(const void *const value, const size_t size)
{
    assert(size == sizeof(void*));
    return (hash_t)*(void**)value;
}


hash_t hash_longlong(const void *const value, const size_t size)
{
    assert(size == sizeof(long long));
    const long long *ll = value;
    return (hash_t)ABS(*ll);
}


hash_t hash_long(const void *const value, const size_t size)
{
    assert(size == sizeof(long));
    const long *l = value;
    return (hash_t)ABS(*l);
}


hash_t hash_int(const void *const value, const size_t size)
{
    assert(size == sizeof(int));
    const int *i = value;
    return (hash_t)(ABS(*i));
}


hash_t hash_short(const void *const value, const size_t size)
{
    assert(size == sizeof(short));
    const short *s = value;
    return (hash_t)ABS(*s);
}


hash_t hash_char(const void *const value, const size_t size)
{
    assert(size == sizeof(char));
    const char *ch = value;
    return (hash_t)ABS(*ch);
}


hash_t hash_ldouble(const void *const value, const size_t size)
{
    assert(size == sizeof(long double));
    const size_t *arr = (size_t*)value;
    return arr[0] + (SMALL_PRIME_1 * arr[1]);
}


hash_t hash_double(const void *const value, const size_t size)
{
    assert(size == sizeof(double));
    const size_t *d = value;
    return *d;
}


hash_t hash_float(const void *const value, const size_t size)
{
    assert(size == sizeof(float));
    return hash_int(value, sizeof(float));
}


hash_t hash_str(const void *const value, const size_t size)
{
    assert(size);
    const char *str = value;
    const size_t rest = size % sizeof(size_t);
    const size_t words_end = size - rest;
    hash_t code = SMALL_PRIME_1;

    size_t i = 0;
    for (; i < words_end; i += sizeof(size_t))
    {
        code = (code + *(hash_t*)&str[i]) * SMALL_PRIME_2;
    }

    for (; i < rest; ++i)
    {
        code = (code + (hash_t)str[i]) * SMALL_PRIME_2;
    }

    return code;
}
