#include "hashmap.h"

#include <stdio.h>

/* Given array of numbers and target sum,
 * find two numbers that add up together,
 * giving a target sum as a result.
 * Return indicies of these two numbers.
 */

typedef struct
{
    size_t i1;
    size_t i2;
}
indices_t;

indices_t two_sum(const vector_t *const numbers, int target)
{
    hashmap_t *map = hm_create(.key_size = sizeof(int),
                               .value_size = sizeof(int),
                               .hashfunc = hash_int);

    for (size_t i = 0; i < vector_capacity(numbers); ++i)
    {
        const int *n = vector_get(numbers, i);
        const int key = target - *n;
        const int *idx = hm_get(map, &key);
        if (idx)
        {
            indices_t res = {.i1 = *idx, .i2 = i};
            hm_destroy(map);
            return res;
        }
        hm_insert(&map, n, &i);
    }

    hm_destroy(map);
    return (indices_t){0};
}

int main(void)
{
    vector_t *numbers = vector_create(.element_size=sizeof(int), .initial_cap=4);

    vector_set(numbers, 0, TMP_REF(int, 2));
    vector_set(numbers, 1, TMP_REF(int, 7));
    vector_set(numbers, 2, TMP_REF(int, 11));
    vector_set(numbers, 3, TMP_REF(int, 15));

    indices_t result = two_sum(numbers, 9);
    vector_destroy(numbers);

    if (result.i1 == 0 && 0 == result.i2)
    {
        printf("no results provided!\n"); 
        return 1;
    }

    printf("result :[%lu, %lu]\n", result.i1, result.i2);
    return 0;
}
