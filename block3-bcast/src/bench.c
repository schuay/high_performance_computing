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
#define DEFAULT_BLOCK_SIZE (1024)
#define ROOT (0)

#define SAFE_BENCH(name, is_random, fn, data, n, comm) do { \
    if (bench(name, is_random, fn, data, n, comm) != 0) { \
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

typedef int (*bcast_t)(int *, int, int, MPI_Comm);

static void
usage(void)
{
    fprintf(stderr, "bcast_bench [-l] [-o] [-a] [-m] [-s seed] [-n length] [-b blocksize] [-r]\n"
                    "   -a: Run the binary broadcast (Default: N)\n"
                    "   -l: Run the linear broadcast (Default: N)\n"
                    "   -m: Run the native MPI broadcast (Default: N)\n"
                    "   -o: Run the binomial broadcast (Default: N)\n"
                    "   -s: The seed used for generating test data (Default: %d)\n"
                    "   -n: The length of the generated test array (Default: %d)\n"
                    "   -b: The block size of pipelined algorithms (Default: %d)\n"
                    "   -r: Use a random communicator (Default: N)\n",
            DEFAULT_SEED, DEFAULT_N, DEFAULT_BLOCK_SIZE);
    exit(EXIT_FAILURE);
}

static int
bench(const char *name,
      const int is_random,
      bcast_t bcast_fn,
      int *data,
      const int n,
      MPI_Comm comm)
{
    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    const double start = MPI_Wtime();
    if (bcast_fn(data, n, ROOT, comm) != MPI_SUCCESS) {
        fprintf(stderr, "%s failed\n", name);
        return -1;
    }
    double local_elapsed = MPI_Wtime() - start;

    double total_elapsed;
    MPI_Reduce(&local_elapsed, &total_elapsed, 1, MPI_DOUBLE, MPI_MAX, ROOT, comm);

    if (rank == ROOT) {
        printf("%s%s,%d,%d,%f\n", name, is_random ? " (rand)" : "", size, n, total_elapsed);
    }

    return 0;
}

int
main(int argc, char **argv)
{
    int algs = 0;
    int seed = DEFAULT_SEED;
    int n = DEFAULT_N;
    int block_size = DEFAULT_BLOCK_SIZE;
    int random_comm = 0;
    int ret = 0;

    int opt;
    while ((opt = getopt(argc, argv, "loamb:s:n:r")) != -1) {
        switch (opt) {
        case 'b':
            block_size = strtol(optarg, NULL, 10);
            if (errno != 0) {
                usage();
            }
            break;
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
        case 'r':
            random_comm = 1;
            break;
        default: /* '?' */
            usage();
        }
    }

    if (optind < argc) {
        usage();
    }

    if (algs == 0) {
        usage();
    }

    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank;
    MPI_Comm_rank(comm, &rank);

    int *data = array_random(seed, n);
    if (data == NULL) {
        ret = -1;
        goto out;
    }

    if (random_comm) {
        srand(rank);
        MPI_Comm_split(MPI_COMM_WORLD, 1, rand(), &comm);
    }

    if (algs & ALG_LINEAR) {
        linear_block_size(block_size);
        SAFE_BENCH("linear", random_comm, bcast_linear, data, n, comm);
    }

    if (algs & ALG_BINOMIAL) {
        SAFE_BENCH("binomial", random_comm, bcast_binomial, data, n, comm);
    }

    if (algs & ALG_BINARY) {
        binary_block_size(block_size);
        SAFE_BENCH("binary", random_comm, bcast_binary, data, n, comm);
    }

    if (algs & ALG_NATIVE) {
        SAFE_BENCH("native", random_comm, bcast_native, data, n, comm);
    }

out:
    if (random_comm) {
        MPI_Comm_free(&comm);
    }

    MPI_Finalize();
    free(data);

    return ret;
}
