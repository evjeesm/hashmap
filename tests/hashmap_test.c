#include "../src/hashmap.h"
#include <check.h>
#include <stdlib.h>

static hashmap_t *map;

static hash_t hash_int(const void *ptr, size_t size)
{
    (void)size;
    return hash(*(int*)ptr);
}

static void setup_empty(void)
{
    hm_create(map,
        .key_size = sizeof(int),
        .value_size = sizeof(int),
        .hashfunc = hash_int
    );
}

static void teardown(void)
{
    hm_destroy(map);
}


START_TEST (test_hm_create)
{
    ck_assert_ptr_nonnull(map);
    ck_assert_uint_eq(hm_count(map), 0);
}
END_TEST


START_TEST (test_hm_insert)
{
    const int key = 2;
    const int value = 100;
    bool retval = hm_insert(&map, &key, &value);
    ck_assert(retval);

    retval = hm_insert(&map, &key, &value);
    ck_assert(!retval);
}
END_TEST


START_TEST (test_hm_insert_full)
{
    int cap = hm_capacity(map);

    // full capacity
    for (int i = 0; i < cap; ++i)
    {
        ck_assert(hm_insert(&map, &i, &i));
    }

    cap = hm_capacity(map);
    size_t count = hm_count(map);
    ck_assert_uint_eq(count, cap);
}
END_TEST


START_TEST (test_hm_insert_rehash)
{
    int old_cap = hm_capacity(map);

    // full capacity
    for (int i = 0; i < old_cap; ++i)
    {
        ck_assert(hm_insert(&map, &i, &i));
    }

    int value = 999;
    hm_insert(&map, &value, &value);

    int new_cap = hm_capacity(map);
    size_t count = hm_count(map);

    ck_assert_uint_eq(new_cap, 512);
    ck_assert_uint_eq(count, old_cap + 1);

    // check that all elements rehashed properly
    for (int i = 0; i < old_cap; ++i)
    {
        ck_assert_mem_eq(hm_get(map, &i), &i, sizeof(int));
    }
    ck_assert_mem_eq(hm_get(map, &value), &value, sizeof(int));
}
END_TEST


START_TEST (test_hm_remove)
{
    const int key = 534;
    const int value = 12;
    ck_assert(hm_insert(&map, &key, &value));
    hm_remove(map, &key);
    ck_assert_ptr_null(hm_get(map, &key));
    ck_assert_uint_eq(hm_count(map), 0);

    const int cap = (int)hm_capacity(map);
    // full capacity
    for (int i = 0; i < cap; ++i)
    {
        ck_assert(hm_insert(&map, &i, &i));
    }

    // delete all
    for (int i = 0; i < cap; ++i)
    {
        hm_remove(map, &i);
        ck_assert_ptr_null(hm_get(map, &i));
    }

    ck_assert_uint_eq(hm_count(map), 0);
}
END_TEST


START_TEST (test_hm_keys_values)
{
    const int expected_cap = 10;
    for (int key = 0; key < expected_cap; ++key)
    {
        int val = key + 10;
        ck_assert(hm_insert(&map, &key, &val));
    }

    vector_t *keys = hm_keys(map);
    vector_t *values = hm_values(map);

    ck_assert_uint_eq(vector_capacity(keys), expected_cap);
    ck_assert_uint_eq(vector_capacity(values), expected_cap);

    for (size_t i = 0; i < vector_capacity(keys); ++i)
    {
        int key = *(int*)vector_get(keys, i);
        int value = *(int*)vector_get(values, i);

        ck_assert_int_eq(value, key + 10);
    }

    vector_destroy(keys);
    vector_destroy(values);
}
END_TEST

Suite *hash_map_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Hash Map");
    
    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup_empty, teardown);
    tcase_add_test(tc_core, test_hm_create);
    tcase_add_test(tc_core, test_hm_insert);
    tcase_add_test(tc_core, test_hm_insert_full);
    tcase_add_test(tc_core, test_hm_insert_rehash);
    tcase_add_test(tc_core, test_hm_remove);
    tcase_add_test(tc_core, test_hm_keys_values);

    suite_add_tcase(s, tc_core);

    return s;
}


int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = hash_map_suite();
    sr = srunner_create(s);

    /* srunner_set_fork_status(sr, CK_NOFORK); */
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

