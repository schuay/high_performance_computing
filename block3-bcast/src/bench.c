#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_SEED (0)
#define DEFAULT_N (1048576)

enum {
    ALG_NATIVE   = 1 << 0,
    ALG_BINARY   = 1 << 1,
    ALG_BINOMIAL = 1 << 2,
    ALG_LINEAR   = 1 << 3,
};

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

int
main(int argc, char **argv)
{
    int algs = 0;
    int seed = DEFAULT_SEED;
    int n = DEFAULT_N;

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

    return 0;
}
