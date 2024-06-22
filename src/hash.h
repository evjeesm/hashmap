#ifndef _HASH_H_
#define _HASH_H_

#include <stddef.h>

#define _SHIFT(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME
#define _COUNT(...) _SHIFT(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)
#define COUNTED_ARGS(...) _COUNT(__VA_ARGS__), __VA_ARGS__
#define TMP_REF(type, value) (type[1]){value}
#define ABS(x) (x < 0 ? -x : x)

typedef size_t hash_t;
typedef hash_t (*hashfunc_t)(const void *const ptr, const size_t size);

hash_t hash_ptr(const void *const value, const size_t size);
hash_t hash_longlong(const void *const value, const size_t size);
hash_t hash_long(const void *const value, const size_t size);
hash_t hash_int(const void *const value, const size_t size);
hash_t hash_short(const void *const value, const size_t size);
hash_t hash_char(const void *const value, const size_t size);
hash_t hash_ldouble(const void *const value, const size_t size);
hash_t hash_double(const void *const value, const size_t size);
hash_t hash_float(const void *const value, const size_t size);
hash_t hash_str(const void *const value, const size_t size);

#define HASHFUNC_FOR(value) \
    _Generic((value), \
        unsigned char: hash_char, \
        char: hash_char, \
        unsigned short: hash_short, \
        short: hash_short, \
        unsigned int: hash_int, \
        int: hash_int, \
        unsigned long: hash_long, \
        long: hash_long, \
        unsigned long long: hash_longlong, \
        long long: hash_longlong, \
        void* : hash_ptr, \
        float: hash_float, \
        double: hash_double, \
        long double: hash_ldouble, \
        char* : hash_str \
        )

#define hash(value) \
    _Generic((value), \
        unsigned char: hash_char, \
        char: hash_char, \
        unsigned short: hash_short, \
        short: hash_short, \
        unsigned int: hash_int, \
        int: hash_int, \
        unsigned long: hash_long, \
        long: hash_long, \
        unsigned long long: hash_longlong, \
        long long: hash_longlong, \
        void* : hash_ptr, \
        float: hash_float, \
        double: hash_double, \
        long double: hash_ldouble \
        )(TMP_REF(__typeof__(value), value), sizeof(value))

#endif/*_HASH_H_*/
