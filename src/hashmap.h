#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include "hash.h"
#include "vector.h"

typedef vector_t hashmap_t;

typedef struct hm_opts
{
    size_t key_size;
    size_t value_size;
    size_t capacity;
    hashfunc_t hashfunc;
    alloc_opts_t alloc_opts; /**< @see vector_opts_t::alloc_opts_t    */
}
hm_opts_t;

typedef enum hm_status_t
{
    HM_SUCCESS = VECTOR_SUCCESS,
    HM_ALREADY_EXISTS = VECTOR_STATUS_LAST
}
hm_status_t;


/*
* The wrapper for `hm_create_` function that provides default values.
*/
#define hm_create(...) \
    hm_create_(&(hm_opts_t){ \
        .capacity = 256, \
        __VA_ARGS__ \
    })

/*
* Creates hashmap
*/
hashmap_t *hm_create_(const hm_opts_t *const opts);


/*
* Release hashmap resources.
*/
void hm_destroy(hashmap_t *const map);


/*
* Duplicates hashmap.
*/
hashmap_t *hm_clone(const hashmap_t *const map);


/*
* Insert new mapping into the hash map.
* Call will fail if mapping for provided key already exists.
*/
hm_status_t hm_insert(hashmap_t **const map, const void *const key, const void *const value);


/*
* Update existing mapping in the hash map.
* Call will fail when mapping for that key is missing.
*/
bool hm_update(hashmap_t *const map, const void *const key, const void *const value);


/*
* Updates value when mapping exists or inserts new mapping otherwise.
*/
hm_status_t hm_upsert(hashmap_t **const map, const void *const key, const void *const value);


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
* Shrink hashmap and perform rehash,
* reserving free space portion of currently stored elements
* (`reserve` param is a positive real number that denotes 
*  how much free space to reserve relative to a current amount of elements being stored:
*    = 0.0f -> do not reserve any space
*    = 1.0f -> reserve as match free space as elements currently stored, etc...
*  ).
*/
hm_status_t hm_shrink_reserve(hashmap_t **const map, const float reserve);


/*
* Returns key's subset.
*/
vector_t *hm_keys(const hashmap_t *const map);


/*
* Returns values's subset.
*/
vector_t *hm_values(const hashmap_t *const map);


#endif/*_HASHMAP_H_*/
