#include <check.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "native.h"
#include "util.h"

#define ROOT (0)
#define SEED (0)
#define N (42)

START_TEST(bcast_1)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *data;

    if (rank == ROOT) {
        data = array_random(SEED, N);
        fail_unless(data != NULL);
    } else {
        data = malloc(N * sizeof(int));
        fail_unless(data != NULL);
    }

    fail_unless(bcast_native(data, N, ROOT, MPI_COMM_WORLD) == MPI_SUCCESS); 

    int *expected = array_random(SEED, N);
    fail_unless(expected != NULL);

    fail_unless(memcmp(expected, data, N * sizeof(int)) == 0);

    free(data);
    free(expected);
}
END_TEST

static Suite *
create_suite(void)
{
    Suite *s = suite_create(__FILE__);
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, bcast_1);

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
