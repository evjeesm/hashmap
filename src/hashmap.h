#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include "hash.h"
#include "vector.h"

typedef vector_t hashmap_t;

typedef struct hm_opts
{
    size_t key_size;
    size_t value_size;
    size_t initial_cap;
    hashfunc_t hashfunc;
}
hm_opts_t;

/*
* The wrapper for `hm_create_` function that provides default values.
*/
#define hm_create(hm_ptr, ...) {\
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Woverride-init\"") \
    hm_create_(&hm_ptr, &(hm_opts_t){ \
        .initial_cap = 256, \
        __VA_ARGS__ \
    }); \
    _Pragma("GCC diagnostic pop") \
}


/*
* Creates hashmap
*/
void hm_create_(hashmap_t **const map, const hm_opts_t *opts);


/*
* Release hashmap resources.
*/
void hm_destroy(hashmap_t *const map);


/*
* Insert new mapping into the hash map.
* Call will fail if mapping for provided key already exists.
*/
bool hm_insert(hashmap_t **const map, const void *const key, const void *const value);


/*
* Update existing mapping in the hash map.
* Call will fail when mapping for that key is missing.
*/
bool hm_update(hashmap_t *const map, const void *const key, const void *const value);


/*
* Updates value when mapping exists or inserts new mapping otherwise.
*/
bool hm_upsert(hashmap_t **const map, const void *const key, const void *const value);


/*
* Remove key from hash map. If key is missing,
* then an operation considered successfull.
*/
void hm_remove(hashmap_t *const map, const void *const key);


/*
* Returns current hashmap capacity.
*/
size_t hm_capacity(const hashmap_t *const map);


/*
* Returns amount of mappings in the map.
*/
size_t hm_count(const hashmap_t *const map);


/*
* Access mapping's value via it's key.
*/
void *hm_get(const hashmap_t *const map, const void *const key);


/*
* Returns key's subset.
*/
vector_t *hm_keys(const hashmap_t *const map);


/*
* Returns values's subset.
*/
vector_t *hm_values(const hashmap_t *const map);


#endif/*_HASHMAP_H_*/
