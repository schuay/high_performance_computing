#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "itree.h"

START_TEST(test_iter_next1)
{
    itree_t t1 = { NULL, NULL, 0, 0, 0 };
    itree_t t2 = { NULL, NULL, 0, 0, 0 };
    itree_t t3 = { &t2, NULL, 0, 0, 0 };
    itree_t t4 = { &t1, &t3, 0, 0, 0 };
    itree_t t5 = { NULL, NULL, 0, 0, 0 };
    itree_t t6 = { NULL, &t5, 0, 0, 0 };
    itree_t t7 = { &t4, &t6, 0, 0, 0 };

    itree_iter_t *iter = itree_iter_init(&t7);

    fail_unless(itree_iter_next(iter) == &t1);
    fail_unless(itree_iter_next(iter) == &t4);
    fail_unless(itree_iter_next(iter) == &t2);
    fail_unless(itree_iter_next(iter) == &t3);
    fail_unless(itree_iter_next(iter) == &t7);
    fail_unless(itree_iter_next(iter) == &t6);
    fail_unless(itree_iter_next(iter) == &t5);
    fail_unless(itree_iter_next(iter) == NULL);

    itree_iter_free(iter);
}
END_TEST

START_TEST(test_insert_into_empty)
{
    fail_unless(1 == 1);
}
END_TEST

static Suite *
create_suite(void)
{
    Suite *s = suite_create("itree");
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, test_insert_into_empty);
    tcase_add_test(tc_core, test_iter_next1);

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
