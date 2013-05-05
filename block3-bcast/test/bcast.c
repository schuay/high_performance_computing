#include <check.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "binary.h"
#include "binomial.h"
#include "linear.h"
#include "native.h"
#include "util.h"

#define BCAST_TEST(name, fn, comm, seed, n, root) \
START_TEST(name) \
{ \
    int rank; \
    MPI_Comm_rank(comm, &rank); \
 \
    int *expected = array_random(seed, n); \
    fail_unless(expected != NULL); \
 \
    int *data = malloc(n * sizeof(int)); \
    fail_unless(data != NULL); \
 \
    if (rank == root) { \
        memcpy(data, expected, n * sizeof(int)); \
    } \
 \
    fail_unless(fn(data, n, root, comm) == MPI_SUCCESS);  \
    fail_unless(memcmp(expected, data, n * sizeof(int)) == 0); \
 \
    free(data); \
    free(expected); \
} \
END_TEST

BCAST_TEST(native01, bcast_native, MPI_COMM_WORLD, 0, 1, 0)
BCAST_TEST(native02, bcast_native, MPI_COMM_WORLD, 0, 42, 0)
BCAST_TEST(native03, bcast_native, MPI_COMM_WORLD, 0, 128, 0)
BCAST_TEST(native04, bcast_native, MPI_COMM_WORLD, 0, 999, 0)
BCAST_TEST(native05, bcast_native, MPI_COMM_WORLD, 0, 1024, 0)
BCAST_TEST(native06, bcast_native, MPI_COMM_WORLD, 0, 1024, 1)
BCAST_TEST(native07, bcast_native, MPI_COMM_WORLD, 0, 1024, 2)
BCAST_TEST(native08, bcast_native, MPI_COMM_WORLD, 0, 1024, 3)
BCAST_TEST(native09, bcast_native, MPI_COMM_WORLD, 0, 1501, 0)

BCAST_TEST(linear01, bcast_linear, MPI_COMM_WORLD, 0, 1, 0)
BCAST_TEST(linear02, bcast_linear, MPI_COMM_WORLD, 0, 42, 0)
BCAST_TEST(linear03, bcast_linear, MPI_COMM_WORLD, 0, 128, 0)
BCAST_TEST(linear04, bcast_linear, MPI_COMM_WORLD, 0, 999, 0)
BCAST_TEST(linear05, bcast_linear, MPI_COMM_WORLD, 0, 1024, 0)
BCAST_TEST(linear06, bcast_linear, MPI_COMM_WORLD, 0, 1024, 1)
BCAST_TEST(linear07, bcast_linear, MPI_COMM_WORLD, 0, 1024, 2)
BCAST_TEST(linear08, bcast_linear, MPI_COMM_WORLD, 0, 1024, 3)
BCAST_TEST(linear09, bcast_linear, MPI_COMM_WORLD, 0, 1501, 0)

BCAST_TEST(binary01, bcast_binary, MPI_COMM_WORLD, 0, 1, 0)
BCAST_TEST(binary02, bcast_binary, MPI_COMM_WORLD, 0, 42, 0)
BCAST_TEST(binary03, bcast_binary, MPI_COMM_WORLD, 0, 128, 0)
BCAST_TEST(binary04, bcast_binary, MPI_COMM_WORLD, 0, 999, 0)
BCAST_TEST(binary05, bcast_binary, MPI_COMM_WORLD, 0, 1024, 0)
BCAST_TEST(binary06, bcast_binary, MPI_COMM_WORLD, 0, 1024, 1)
BCAST_TEST(binary07, bcast_binary, MPI_COMM_WORLD, 0, 1024, 2)
BCAST_TEST(binary08, bcast_binary, MPI_COMM_WORLD, 0, 1024, 3)
BCAST_TEST(binary09, bcast_binary, MPI_COMM_WORLD, 0, 1501, 0)

BCAST_TEST(binomial01, bcast_binomial, MPI_COMM_WORLD, 0, 1, 0)
BCAST_TEST(binomial02, bcast_binomial, MPI_COMM_WORLD, 0, 42, 0)
BCAST_TEST(binomial03, bcast_binomial, MPI_COMM_WORLD, 0, 128, 0)
BCAST_TEST(binomial04, bcast_binomial, MPI_COMM_WORLD, 0, 999, 0)
BCAST_TEST(binomial05, bcast_binomial, MPI_COMM_WORLD, 0, 1024, 0)
BCAST_TEST(binomial06, bcast_binomial, MPI_COMM_WORLD, 0, 1024, 1)
BCAST_TEST(binomial07, bcast_binomial, MPI_COMM_WORLD, 0, 1024, 2)
BCAST_TEST(binomial08, bcast_binomial, MPI_COMM_WORLD, 0, 1024, 3)
BCAST_TEST(binomial09, bcast_binomial, MPI_COMM_WORLD, 0, 1501, 0)

static Suite *
create_suite(void)
{
    Suite *s = suite_create(__FILE__);
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, native01);
    tcase_add_test(tc_core, native02);
    tcase_add_test(tc_core, native03);
    tcase_add_test(tc_core, native04);
    tcase_add_test(tc_core, native05);
    tcase_add_test(tc_core, native06);
    tcase_add_test(tc_core, native07);
    tcase_add_test(tc_core, native08);
    tcase_add_test(tc_core, native09);

    tcase_add_test(tc_core, linear01);
    tcase_add_test(tc_core, linear02);
    tcase_add_test(tc_core, linear03);
    tcase_add_test(tc_core, linear04);
    tcase_add_test(tc_core, linear05);
    tcase_add_test(tc_core, linear06);
    tcase_add_test(tc_core, linear07);
    tcase_add_test(tc_core, linear08);
    tcase_add_test(tc_core, linear09);

    tcase_add_test(tc_core, binary01);
    tcase_add_test(tc_core, binary02);
    tcase_add_test(tc_core, binary03);
    tcase_add_test(tc_core, binary04);
    tcase_add_test(tc_core, binary05);
    tcase_add_test(tc_core, binary06);
    tcase_add_test(tc_core, binary07);
    tcase_add_test(tc_core, binary08);
    tcase_add_test(tc_core, binary09);

    tcase_add_test(tc_core, binomial01);
    tcase_add_test(tc_core, binomial02);
    tcase_add_test(tc_core, binomial03);
    tcase_add_test(tc_core, binomial04);
    tcase_add_test(tc_core, binomial05);
    tcase_add_test(tc_core, binomial06);
    tcase_add_test(tc_core, binomial07);
    tcase_add_test(tc_core, binomial08);
    tcase_add_test(tc_core, binomial09);

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
