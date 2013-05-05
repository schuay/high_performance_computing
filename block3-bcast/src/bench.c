#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "binary.h"
#include "binomial.h"
#include "linear.h"
#include "native.h"
#include "util.h"

#define DEFAULT_SEED (0)
#define DEFAULT_N (1048576)
#define ROOT (0)

#define SAFE_BENCH(name, fn, data, n) do { \
    if (bench(name, fn, data, n) != 0) { \
        ret = -1; \
        goto out; \
    } \
} while (0);

enum {
    ALG_NATIVE   = 1 << 0,
    ALG_BINARY   = 1 << 1,
    ALG_BINOMIAL = 1 << 2,
    ALG_LINEAR   = 1 << 3,
};

typedef int (*bcast_t)(void *, int, int, MPI_Comm);

static void
usage(void)
{
    fprintf(stderr, "bcast_bench [-l] [-o] [-a] [-m] [-s seed] [-n length]\n"
                    "   -a: Run the binary broadcast (Default: N)\n"
                    "   -l: Run the linear broadcast (Default: N)\n"
                    "   -m: Run the native MPI broadcast (Default: N)\n"
                    "   -o: Run the binomial broadcast (Default: N)\n"
                    "   -s: The seed used for generating test data (Default: %d)\n"
                    "   -n: The length of the generated test array (Default: %d)\n",
            DEFAULT_SEED, DEFAULT_N);
    exit(EXIT_FAILURE);
}

static int
bench(const char *name,
      bcast_t bcast_fn,
      int *data,
      const int n)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const double start = MPI_Wtime();
    if (bcast_fn(data, n, ROOT, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fprintf(stderr, "%s failed\n", name);
        return -1;
    }
    double local_elapsed = MPI_Wtime() - start;

    double total_elapsed;
    MPI_Reduce(&local_elapsed, &total_elapsed, 1, MPI_DOUBLE, MPI_MAX, ROOT, MPI_COMM_WORLD);

    if (rank == ROOT) {
        printf("%s, %d, %f\n", name, n, total_elapsed);
    }

    return 0;
}

int
main(int argc, char **argv)
{
    int algs = 0;
    int seed = DEFAULT_SEED;
    int n = DEFAULT_N;
    int ret = 0;

    int opt;
    while ((opt = getopt(argc, argv, "loams:n:")) != -1) {
        switch (opt) {
        case 's':
            seed = strtol(optarg, NULL, 10);
            if (errno != 0) {
                usage();
            }
            break;
        case 'n':
            n = strtol(optarg, NULL, 10);
            if (errno != 0) {
                usage();
            }
            break;
        case 'l':
            algs |= ALG_LINEAR;
            break;
        case 'a':
            algs |= ALG_BINARY;
            break;
        case 'o':
            algs |= ALG_BINOMIAL;
            break;
        case 'm':
            algs |= ALG_NATIVE;
            break;
        default: /* '?' */
            usage();
        }
    }

    if (optind < argc) {
        usage();
    }

    /* TODO: Non-well behaved communicator benchmarks. */

    int *data = array_random(seed, n);
    if (data == NULL) {
        ret = -1;
        goto out;
    }

    MPI_Init(&argc, &argv);

    if (algs & ALG_LINEAR) {
        SAFE_BENCH("linear", bcast_linear, data, n);
    }

    if (algs & ALG_BINOMIAL) {
        SAFE_BENCH("binomial", bcast_binomial, data, n);
    }

    if (algs & ALG_BINARY) {
        SAFE_BENCH("binary", bcast_binary, data, n);
    }

    if (algs & ALG_NATIVE) {
        SAFE_BENCH("native", bcast_native, data, n);
    }

out:
    MPI_Finalize();
    free(data);

    return ret;
}
