#ifndef __HASH_H
#define __HASH_H

#include <stdint.h>

#define HASH_NOT_FOUND (UINT32_MAX)

struct __hash_t;
typedef struct __hash_t hash_t;

/**
 * Creates and initializes a hash structure, and returns a pointer to it.
 * Returns NULL on error.
 */
hash_t *
hash_init(void);

/**
 * Inserts the key-value pair (key, in) into the hash table
 * and writes the previous value associated with key into out.
 * If the key did not previously exist in the hash table,
 * HASH_NOT_FOUND is written into out instead.
 * Returns 0 on success, < 0 on error.
 */
int
hash_insert(hash_t *hash,
            const uint64_t key,
            const uint64_t in,
            uint64_t *out);

void
hash_free(hash_t *hash);

#endif /* __HASH_H */
