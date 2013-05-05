#include "util.h"

#include <stdlib.h>

int *
array_random(const int seed,
             const int n)
{
    int *out = malloc(n * sizeof(int));
    if (out == NULL) {
        return NULL;
    }

    srand(seed);

    for (int i = 0; i < n; i++) {
        out[i] = rand();
    }

    return out;
}
