#ifndef __STACKDIST_H
#define __STACKDIST_H

#include <stdint.h>

#define STACKDIST_INFINITE (UINT64_MAX)

typedef void (*stackdist_cb)(uint64_t index, uint64_t distance);

/**
 * Processes the memory trace located at p with a length of n bytes.
 * For each memory location, the fn callback is called with the current
 * index and the associated stack distance.
 * Returns 0 on success, < 0 on error.
 */
int
stackdist_process_trace(const uint64_t *p,
                        const uint64_t n,
                        const stackdist_cb fn);

#endif /* __STACKDIST_H */
