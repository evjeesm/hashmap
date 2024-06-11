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
    map = hm_create(
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


Suite *hash_map_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Hash Map");
    
    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup_empty, teardown);
    tcase_add_test(tc_core, test_hm_create);

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

