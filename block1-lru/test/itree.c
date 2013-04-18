#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "itree.h"

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
