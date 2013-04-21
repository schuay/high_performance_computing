#ifndef __NAIVE_H
#define __NAIVE_H

#include <stdint.h>

#define NAIVE_INFINITE (UINT64_MAX)

typedef void (*naive_cb)(uint64_t index, uint64_t distance);

/**
 * Processes the memory trace located at p with a length of n bytes.
 * For each memory location, the fn callback is called with the current
 * index and the associated stack distance.
 * Returns 0 on success, < 0 on error.
 */
int
naive_process_trace(const uint64_t *p,
                    const uint64_t n,
                    const naive_cb fn);

#endif /* __NAIVE_H */
