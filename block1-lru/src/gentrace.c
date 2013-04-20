#include <errno.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum {
    MODE_PRINT,
    MODE_BINARY
};

static void
usage(void)
{
    fprintf(stderr, "gentrace [-s seed] [-p] -m domain -n count\n"
                    "   -p: Prints values as strings instead of binary\n");
    exit(EXIT_FAILURE);
}

int
main(int argc, char **argv)
{
    uint64_t seed = 0, domain = 0, count = 0;
    int mode = MODE_BINARY;

    int opt;
    while ((opt = getopt(argc, argv, "s:m:n:p")) != -1) {
        switch (opt) {
            case 's':
                seed = strtoull(optarg, NULL, 10);
                if (errno != 0) {
                    usage();
                }
                break;
            case 'm':
                domain = strtoull(optarg, NULL, 10);
                if (errno != 0) {
                    usage();
                }
                break;
            case 'n':
                count = strtoull(optarg, NULL, 10);
                if (errno != 0) {
                    usage();
                }
                break;
            case 'p':
                mode = MODE_PRINT;
                break;
            default: /* '?' */
                usage();
        }
    }

    if (optind < argc) {
        usage();
    }

    if (domain == 0 || count == 0) {
        usage();
    }

    srand(seed);

    if (mode == MODE_PRINT) {
        for (uint64_t i = 0; i < count; i++) {
            printf("%llu\n", (unsigned long long)(rand() % domain));
        }
    } else if (mode == MODE_BINARY) {
        for (uint64_t i = 0; i < count; i++) {
            uint64_t next = rand() % domain;
            fwrite(&next, sizeof(next), 1, stdout);
        }
        fflush(stdout);
    }

    return EXIT_SUCCESS;
}
