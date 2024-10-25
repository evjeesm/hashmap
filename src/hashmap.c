#include "hashmap.h"
#include "bitset.h"
#include "vector.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define ALIGNMENT sizeof(size_t)
#define BIT_FIELD_LEN 2
#define LARGE_PRIME 0x7fffffffu

typedef struct hm_header
{
    alloc_opts_t alloc_opts;
    size_t key_size;
    size_t aligned_key_size;
    size_t value_size;
    hashfunc_t hashfunc;

    unsigned int a; /* random factors for multiplicative hashing */
    unsigned int b;
    char usage_tbl[];
}
hm_header_t;

typedef enum hm_slot_status
{
    HM_SLOT_UNUSED = 0,
    HM_SLOT_USED,
    HM_SLOT_DELETED
}
hm_slot_status_t;

/***                          ***
* === forward declarations  === *
***                          ***/

static size_t calc_usage_tbl_size(const size_t capacity);

static hm_header_t *get_hm_header(const hashmap_t *const map);

static size_t hash_to_index(const hm_header_t *header, const hash_t hash, const size_t capacity);
static void set_entry(hashmap_t *const map, const size_t index, const void *const key, const void *const value);
static char *get_key(const hashmap_t *const map, const size_t index);
static char *get_value(const hashmap_t *const map, const size_t index);

static void randomize_factors(hm_header_t *const header);
static hm_status_t rehash(hashmap_t **const map, const size_t new_cap);

/***                       ***
* === API implementation === *
***                       ***/

hashmap_t *hm_create_(const hm_opts_t *const opts)
{
    assert(opts);
    assert(opts->key_size && "key_size wasn't provided");
    assert(opts->value_size && "value_size wasn't provided");
    assert(opts->hashfunc && "hashfunc wasn't provided");

    const size_t aligned_key_size = calc_aligned_size(opts->key_size, ALIGNMENT);
    const size_t aligned_value_size = calc_aligned_size(opts->value_size, ALIGNMENT);
    const size_t usage_tbl_size = calc_usage_tbl_size(opts->capacity);

    /* allocate storage for hashmap */
    hashmap_t *map = vector_create(
        .ext_header_size = sizeof(hm_header_t) + usage_tbl_size,
        .initial_cap = opts->capacity,
        .element_size = aligned_key_size + aligned_value_size,
        .alloc_opts = opts->alloc_opts,
    );

    if (!map) return NULL;

    /* initializing hashmap related data */
    hm_header_t *header = get_hm_header(map);

    *header = (hm_header_t){
        .alloc_opts = opts->alloc_opts,
        .key_size = opts->key_size,
        .aligned_key_size = aligned_key_size,
        .value_size = opts->value_size,
        .hashfunc = opts->hashfunc,
    };

    bitset_init(header->usage_tbl, usage_tbl_size);
    randomize_factors(header);

    return map;
}


hashmap_t *hm_clone(const hashmap_t *const map)
{
    assert(map);
    return vector_clone(map);
}


void hm_destroy(hashmap_t *const map)
{
    assert(map);
    vector_destroy(map);
}


hm_status_t hm_insert(hashmap_t **const map, const void *key, const void *value)
{
    assert(map && *map);
    assert(key);
    assert(value);

    hm_header_t* header = get_hm_header(*map);
    const size_t capacity = hm_capacity(*map);
    const size_t start_index = hash_to_index(header,
        header->hashfunc(key, header->key_size),
        capacity);

    for (size_t i = 0; i < capacity; ++i)
    {
        const size_t index = (i + start_index) % capacity;
        const hm_slot_status_t slot_stat = bitset_test(header->usage_tbl, BIT_FIELD_LEN, index);
        if (HM_SLOT_USED != slot_stat)
        {
            bitset_set(header->usage_tbl, BIT_FIELD_LEN, index, HM_SLOT_USED);
            set_entry(*map, index, key, value);
            return HM_SUCCESS;
        }
        else if (0 == memcmp(key, get_key(*map, index), header->key_size))
        {
            return HM_ALREADY_EXISTS;
        }
    }

    hm_status_t status = rehash(map, 2 * hm_capacity(*map));
    if (HM_SUCCESS != status) return status;

    (void) hm_insert(map, key, value);
    return HM_SUCCESS;
}


bool hm_update(hashmap_t *const map, const void *const key, const void *const value)
{
    assert(map);
    assert(key);
    assert(value);

    void *old_value = hm_get(map, key);
    if (!old_value) return false;

    const hm_header_t *header = get_hm_header(map);
    memcpy(old_value, value, header->value_size);
    return true;
}


hm_status_t hm_upsert(hashmap_t **const map, const void *const key, const void *const value)
{
    assert(map && *map);
    assert(key);
    assert(value);

    hm_header_t* header = get_hm_header(*map);
    const size_t capacity = vector_capacity(*map);
    const size_t start_index = hash_to_index(header,
        header->hashfunc(key, header->key_size),
        capacity);

    for (size_t i = start_index; i < (capacity + start_index); ++i)
    {
        const size_t index = i % capacity;
        const hm_slot_status_t slot_stat = bitset_test(header->usage_tbl, BIT_FIELD_LEN, index);

        switch (slot_stat)
        {
            case HM_SLOT_UNUSED:
                bitset_set(header->usage_tbl, BIT_FIELD_LEN, index, HM_SLOT_USED);
                memcpy(get_value(*map, index), value, header->value_size);
                return HM_SUCCESS;

            case HM_SLOT_USED:
                if (0 == memcmp(key, get_key(*map, index), header->key_size))
                {
                    memcpy(get_value(*map, index), value, header->value_size);
                    return HM_SUCCESS; 
                }
                break;

            case HM_SLOT_DELETED:
                continue;
        }
    }

    hm_status_t status = rehash(map, 2 * hm_capacity(*map));
    if (HM_SUCCESS != status) return status;

    (void)hm_insert(map, key, value);
    return HM_SUCCESS;
}


void hm_remove(hashmap_t *const map, const void *const key)
{
    assert(map);
    assert(key);

    hm_header_t* header = get_hm_header(map);
    const size_t capacity = vector_capacity(map);
    const size_t start_index = hash_to_index(header,
        header->hashfunc(key, header->key_size),
        capacity);

    for (size_t i = 0; i < capacity; ++i)
    {
        const size_t index = (i + start_index) % capacity;
        const hm_slot_status_t slot_stat = bitset_test(header->usage_tbl, BIT_FIELD_LEN, index);
        switch (slot_stat)
        {
            case HM_SLOT_UNUSED:
                return;

            case HM_SLOT_USED:
                if (0 == memcmp(key, get_key(map, index), header->key_size))
                {
                    bitset_set(header->usage_tbl, BIT_FIELD_LEN, index, HM_SLOT_DELETED);
                    return;
                }
                break;

            case HM_SLOT_DELETED:
                continue;
        }
    }
}


size_t hm_capacity(const hashmap_t *const map)
{
    assert(map);

    return vector_capacity(map);
}


size_t hm_count(const hashmap_t *const map)
{
    assert(map);

    const size_t capacity = hm_capacity(map);
    const hm_header_t *header = get_hm_header(map);
    size_t count = 0;

    for (size_t i = 0; i < capacity; ++i)
    {
        if (HM_SLOT_USED == bitset_test(header->usage_tbl, BIT_FIELD_LEN, i))
        {
            ++count;
        }
    }
    return count;
}


void *hm_get(const hashmap_t *const map, const void *const key)
{
    assert(map);
    assert(key);

    const hm_header_t* header = get_hm_header(map);
    const size_t capacity = vector_capacity(map);
    const size_t start_index = hash_to_index(header,
        header->hashfunc(key, header->key_size),
        capacity);

    for (size_t i = 0; i < capacity; ++i)
    {
        const size_t index = (i + start_index) % capacity;
        const hm_slot_status_t slot_stat = bitset_test(header->usage_tbl, BIT_FIELD_LEN, index);

        switch (slot_stat)
        {
            case HM_SLOT_UNUSED:
                return NULL;

            case HM_SLOT_USED:
                if (0 == memcmp(key, get_key(map, index), header->key_size))
                {
                    return get_value(map, index);
                }
                break;

            case HM_SLOT_DELETED:
                continue;
        }
    }

    return NULL;
}


hm_status_t hm_shrink_reserve(hashmap_t **const map, const float reserve)
{
    assert(map && *map);
    assert(reserve >= 0.0f);

    const size_t count = hm_count(*map);
    const size_t new_cap = count * (1.0f + reserve);

    return rehash(map, new_cap);
}


vector_t *hm_keys(const hashmap_t *const map)
{
    assert(map);

    const hm_header_t *header = get_hm_header(map);
    const size_t capacity = hm_capacity(map);

    vector_t *keys = vector_create(
        .element_size = header->aligned_key_size,
        .initial_cap = hm_count(map)
    );

    if (!keys) return NULL;

    for (size_t slot = 0, key = 0; slot < capacity; ++slot)
    {
        if (HM_SLOT_USED == bitset_test(header->usage_tbl, BIT_FIELD_LEN, slot))
        {
            vector_set(keys, key++, get_key(map, slot));
        }
    }

    return keys;
}


vector_t *hm_values(const hashmap_t *const map)
{
    assert(map);

    const hm_header_t *header = get_hm_header(map);
    const size_t capacity = hm_capacity(map);

    vector_t *values = vector_create(
        .element_size = calc_aligned_size(header->value_size, ALIGNMENT),
        .initial_cap = hm_count(map));

    if (!values) return NULL;

    for (size_t slot = 0, value = 0; slot < capacity; ++slot)
    {
        if (HM_SLOT_USED == bitset_test(header->usage_tbl, BIT_FIELD_LEN, slot))
        {
            vector_set(values, value++, get_value(map, slot));
        }
    }

    return values;
}


/***                     ***
* === static functions === *
***                     ***/

static size_t calc_usage_tbl_size(const size_t capacity)
{
    return calc_aligned_size(capacity * BIT_FIELD_LEN / BYTE, ALIGNMENT);
}


/*
* Function gives an access to the hash map header that is allocated 
* after vector's control struct.
*/
static hm_header_t *get_hm_header(const hashmap_t *const map)
{
    return (hm_header_t*)vector_get_ext_header(map);
}


static char *get_key(const hashmap_t *const map, const size_t index)
{
    return (char*)vector_get(map, index);
}


static char *get_value(const hashmap_t *const map, const size_t index)
{
    const hm_header_t *header = get_hm_header(map);
    return get_key(map, index) + header->aligned_key_size;
}


static void set_entry(hashmap_t *const map, const size_t index, const void *const key, const void *const value)
{
    const hm_header_t *header = get_hm_header(map);
    char *entry = (char*) vector_get(map, index);
    memcpy(entry, key, header->key_size);
    memcpy(entry + header->aligned_key_size, value, header->value_size);
}


/*
* `a` and `b` factors used in conversion of the hash code into index.
* randomization makes hash function less pridictable.
*/
static void randomize_factors(hm_header_t *const header)
{
    header->a = (rand() % (LARGE_PRIME-1)) + 1;  /* [1, p-1] */
    header->b = (rand() % (LARGE_PRIME));        /* [0, p-1] */
}


/*
* Calculates index utilizing multiplicative hashing. (a*h + b) mod p mod c
*/
static size_t hash_to_index(const hm_header_t *header, const hash_t hash, const size_t capacity)
{
    return ((header->a * hash + header->b) % LARGE_PRIME) % capacity;
}


static hm_status_t rehash(hashmap_t **const map, const size_t new_cap)
{
    assert(new_cap >= hm_count(*map));

    const hm_header_t *old_header = get_hm_header(*map);
    const size_t prev_capacity = hm_capacity(*map);

    hashmap_t *new = hm_create(
        .capacity = new_cap,
        .key_size = old_header->key_size,
        .value_size = old_header->value_size,
        .hashfunc = old_header->hashfunc,
        .alloc_opts = old_header->alloc_opts,
    );

    if (!new) return (hm_status_t)VECTOR_ALLOC_ERROR;

    for (size_t i = 0; i < prev_capacity; ++i)
    {
        if (HM_SLOT_USED == bitset_test(old_header->usage_tbl, BIT_FIELD_LEN, i))
        {
            (void) hm_insert(&new, get_key(*map, i), get_value(*map, i)); /* always succeedes */
        }
    }

    hm_destroy(*map);
    *map = new;
    return HM_SUCCESS;
}

