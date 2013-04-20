#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "stackdist.h"

static void
stackdist_callback(uint64_t index,
                   uint64_t distance)
{
    printf("%llu: %llu\n", (long long unsigned)index, (long long unsigned)distance);
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

    const int n = sb.st_size / sizeof(uint64_t);
    stackdist_process_trace(p, n, stackdist_callback);

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
