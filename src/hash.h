#ifndef _HASH_H_
#define _HASH_H_

#include <stddef.h>

#define _SHIFT(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME
#define _COUNT(...) _SHIFT(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)
#define COUNTED_ARGS(...) _COUNT(__VA_ARGS__), __VA_ARGS__

typedef size_t hash_t;
typedef hash_t (*hashfunc_t)(const void *ptr, size_t size);

hash_t hash_str(const char *str);
hash_t hash_strn(const char *str, size_t size);
hash_t hash_llong(long long value);
hash_t hash_ldouble(long double value);

#define hash(value, ...) \
    _Generic((value), \
        unsigned char: hash_llong, \
        char: hash_llong, \
        unsigned short: hash_llong, \
        short: hash_llong, \
        unsigned int: hash_llong, \
        int: hash_llong, \
        unsigned long: hash_llong, \
        long: hash_llong, \
        unsigned long long: hash_llong, \
        long long: hash_llong, \
        void* : hash_llong, \
        float: hash_ldouble, \
        double: hash_ldouble, \
        long double: hash_ldouble, \
        const char *: hash_str, \
        char* : hash_strn \
        ) \
    (value ,##__VA_ARGS__)

#endif/*_HASH_H_*/
