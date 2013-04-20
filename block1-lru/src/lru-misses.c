#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "stackdist.h"

#define SUMS_COUNT (65)

static uint64_t distance_sums[SUMS_COUNT];

static void
stackdist_callback(uint64_t index __attribute__ ((unused)),
                   uint64_t distance)
{
    for (int exp = 0; exp < SUMS_COUNT; exp++) {
        const uint64_t mask = ~((1ULL << exp) - 1ULL);
        if ((mask & distance) == 0) {
            break;
        }
        distance_sums[exp]++;
    }
}

static void
print_distance_sums(const uint64_t *sums,
                    const int n)
{
    for (int i = 0; i < n; i++) {
        printf("%llu\n", (long long unsigned)sums[i]);
    }
}

static void
usage(void)
{
    fprintf(stderr, "usage: lru-misses filename\n");
    exit(EXIT_FAILURE);
}

int
main(int argc, char **argv)
{
    if (argc != 2) {
        usage();
    }
    
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        return EXIT_FAILURE;
    }

    void *p = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (p == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    memset(distance_sums, 0, sizeof(distance_sums));

    const int n = sb.st_size / sizeof(uint64_t);
    stackdist_process_trace(p, n, stackdist_callback);

    print_distance_sums(distance_sums, sizeof(distance_sums) / sizeof(distance_sums[0]));

    if (munmap(p, sb.st_size) == -1) {
        perror("munmap");
        return EXIT_FAILURE;
    }

    if (close(fd) == -1) {
        perror("close");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
