#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "naive.h"
#include "stackdist.h"

#define SI (STACKDIST_INFINITE)

static uint64_t trace[] = { 0, 1, 2, 3, 4, 5, 6, 3, 7, 8, 3, 0, 5,10, 4, 3, 4, 4, 4 };
static uint64_t expct[] = {SI,SI,SI,SI,SI,SI,SI, 3,SI,SI, 2, 8, 5,SI, 7, 4, 1, 0, 0 };
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
    fail_unless(memcmp(expct, actual, sizeof(trace)) == 0);
}
END_TEST

static uint64_t *t;
static uint64_t *naive;
static uint64_t *lru;

static void
lru_callback(uint64_t index, uint64_t distance)
{
    lru[index] = distance;
}

static void
naive_callback(uint64_t index, uint64_t distance)
{
    naive[index] = distance;
}

static int
tracecmp(const uint64_t *naive,
         const uint64_t *lru,
         const uint64_t *trace,
         const uint64_t n)
{
    int not_equal = 0;
    for (uint64_t i = 0; i < n; i++) {
        if (naive[i] == lru[i]) {
            continue;
        }
        not_equal = 1;
        fprintf(stderr, "i: %llu, t[i]: %llu, naive: %llu, lru: %llu\n",
                (long long unsigned)i,
                (long long unsigned)trace[i],
                (long long unsigned)naive[i],
                (long long unsigned)lru[i]);
    }
    return not_equal;
}

START_TEST(test_2)
{
    const uint64_t n = 128;
    const uint64_t domain = 32;

    t = malloc(n * sizeof(uint64_t));
    naive = malloc(n * sizeof(uint64_t));
    lru = malloc(n * sizeof(uint64_t));

    srand(10);
    for (uint64_t i = 0; i < n; i++) {
        t[i] = rand() % domain;
    }

    fail_unless(naive_process_trace(t, n, naive_callback) == 0);
    fail_unless(stackdist_process_trace(t, n, lru_callback) == 0);

    fail_unless(tracecmp(naive, lru, t, n) == 0);

    free(t);
    free(naive);
    free(lru);
}
END_TEST

static Suite *
create_suite(void)
{
    Suite *s = suite_create(__FILE__);
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, test_1);
    tcase_add_test(tc_core, test_2);

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
