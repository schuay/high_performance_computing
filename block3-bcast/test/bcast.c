#include <check.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "native.h"
#include "util.h"

#define BCAST_TEST(name, fn, comm, seed, n, root) \
START_TEST(name) \
{ \
    int rank; \
    MPI_Comm_rank(comm, &rank); \
 \
    int *data; \
 \
    if (rank == root) { \
        data = array_random(seed, n); \
        fail_unless(data != NULL); \
    } else { \
        data = malloc(n * sizeof(int)); \
        fail_unless(data != NULL); \
    } \
 \
    fail_unless(fn(data, n, root, comm) == MPI_SUCCESS);  \
 \
    int *expected = array_random(seed, n); \
    fail_unless(expected != NULL); \
 \
    fail_unless(memcmp(expected, data, n * sizeof(int)) == 0); \
 \
    free(data); \
    free(expected); \
} \
END_TEST

BCAST_TEST(native01, bcast_native, MPI_COMM_WORLD, 0, 42, 0)

static Suite *
create_suite(void)
{
    Suite *s = suite_create(__FILE__);
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, native01);

    suite_add_tcase(s, tc_core);

    return s;
}

int
main(int argc,
     char **argv)
{
    MPI_Init(&argc, &argv);

    int number_failed;
    Suite *s = create_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    MPI_Finalize();

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
