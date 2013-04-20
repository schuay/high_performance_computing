#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "hash.h"

START_TEST(test_init)
{
    hash_t *h;
    fail_unless((h = hash_init()) != NULL);
    hash_free(h);
}
END_TEST

START_TEST(test_insert1)
{
    uint64_t out;
    hash_t *h;
    fail_unless((h = hash_init()) != NULL);
    fail_unless(hash_insert(h, 1, 1, &out) == 0);
    fail_unless(out == HASH_NOT_FOUND);
    hash_free(h);
}
END_TEST

START_TEST(test_insert2)
{
    uint64_t out;
    hash_t *h;
    fail_unless((h = hash_init()) != NULL);
    fail_unless(hash_insert(h, 1, 1, &out) == 0);
    fail_unless(hash_insert(h, 1, 2, &out) == 0);
    fail_unless(out == 1);
    hash_free(h);
}
END_TEST

START_TEST(test_insert3)
{
    uint64_t out;
    hash_t *h;
    fail_unless((h = hash_init()) != NULL);
    fail_unless(hash_insert(h, 1, 1, &out) == 0);
    fail_unless(hash_insert(h, 2, 2, &out) == 0);
    fail_unless(out == HASH_NOT_FOUND);
    fail_unless(hash_insert(h, 1, 3, &out) == 0);
    fail_unless(out == 1);
    fail_unless(hash_insert(h, 2, 4, &out) == 0);
    fail_unless(out == 2);
    hash_free(h);
}
END_TEST

START_TEST(test_insert4)
{
    uint64_t out;
    hash_t *h;
    fail_unless((h = hash_init()) != NULL);
    fail_unless(hash_insert(h, 1, 1, &out) == 0);
    fail_unless(hash_insert(h, 2, 2, &out) == 0);
    fail_unless(out == HASH_NOT_FOUND);
    fail_unless(hash_insert(h, 1, 3, &out) == 0);
    fail_unless(out == 1);
    fail_unless(hash_insert(h, 2, 4, &out) == 0);
    fail_unless(out == 2);
    fail_unless(hash_insert(h, 1, 5, &out) == 0);
    fail_unless(out == 3);
    hash_free(h);
}
END_TEST

static Suite *
create_suite(void)
{
    Suite *s = suite_create("hash");
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, test_init);
    tcase_add_test(tc_core, test_insert1);
    tcase_add_test(tc_core, test_insert2);
    tcase_add_test(tc_core, test_insert3);
    tcase_add_test(tc_core, test_insert4);

    suite_add_tcase(s, tc_core);

    return s;
}

int
main(int argc __attribute__ ((unused)),
     char **argv __attribute__ ((unused)))
{
    int number_failed;
    Suite *s = create_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
