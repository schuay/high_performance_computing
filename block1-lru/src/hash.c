#include "hash.h"

#include "uthash.h"

typedef struct {
    uint32_t k;         /* The key. */
    uint32_t v;         /* The value. */
    UT_hash_handle hh;  /* The hash handle. */
} hash_entry_t;

struct __hash_t {
    hash_entry_t *h;
};

hash_t *
hash_init(void)
{
    return NULL;
}

int
hash_insert(hash_t *hash,
            const uint32_t key,
            const uint32_t in,
            uint32_t *out)
{
    return -1;
}

void
hash_free(hash_t *hash)
{
}
