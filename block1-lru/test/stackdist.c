#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "stackdist.h"

#define SI (STACKDIST_INFINITE)

static uint64_t trace[] = { 0, 1, 2, 3, 4, 5, 6, 3, 7, 8, 3, 0, 5,10, 4, 3, 4, 4, 4 };
static uint64_t expct[] = {SI,SI,SI,SI,SI,SI,SI, 3,SI,SI, 2, 8, 4,SI, 6, 3, 1, 0, 0 };
static uint64_t actual[sizeof(expct) / sizeof(expct[0])];

static void
stackdist_callback(uint64_t index,
                   uint64_t distance)
{
    actual[index] = distance;
}

START_TEST(test_1)
{
    const int n = sizeof(trace) / sizeof(trace[0]);
    fail_unless(stackdist_process_trace(trace, n, stackdist_callback) == 0);
    fail_unless(memcmp(expct, actual, n) == 0);
}
END_TEST

static Suite *
create_suite(void)
{
    Suite *s = suite_create(__FILE__);
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, test_1);

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
