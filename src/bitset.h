#ifndef _BITSET_H_
#define _BITSET_H_
#include <string.h>

#define BYTE 8
#define NIBBLE 4
#define CRUMB 2

#define bitset_init(memory, size) do { memset(memory, 0, size); } while(0)

#define byte_for(field_size, index) (index / (BYTE / field_size))

#define shift_for(field_size, index) (index * field_size % BYTE)

#define field_mask(field_size) ((1 << field_size) - 1)

#define bitset_test(memory, field_size, index) \
    (field_mask(field_size) & (memory[byte_for(field_size, index)] >> shift_for(field_size, index)))

#define bitset_set(memory, field_size, index, value) \
    ( memory[byte_for(field_size, index)] ^= (bitset_test(memory, field_size, index) ^ (char)value) << shift_for(field_size, index) )

#define bitset_to_str(memory, buf, size, spacing) do {\
    buf[0] = '\0'; \
    for (size_t i = 0; i < size; ++i) \
    {\
        if (spacing && !(i % spacing)) strcat(buf, " "); \
        char bit = bitset_test(memory, 1, i); \
        strcat(buf, bit ? "1" : "0"); \
    } \
} while (0)


#endif/*_BITSET_H_*/


