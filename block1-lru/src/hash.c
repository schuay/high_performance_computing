#include "hash.h"

#include "uthash.h"

typedef struct {
    uint64_t k;         /* The key. */
    uint64_t v;         /* The value. */
    UT_hash_handle hh;  /* The hash handle. */
} hash_entry_t;

struct __hash_t {
    hash_entry_t *h;
};

hash_t *
hash_init(void)
{
    return calloc(1, sizeof(hash_t));
}

int
hash_insert(hash_t *hash,
            const uint64_t key,
            const uint64_t in,
            uint64_t *out)
{
    hash_entry_t *e = malloc(sizeof(hash_entry_t));
    if (e == NULL) {
        return -1;
    }

    e->k = key;
    e->v = in;

    hash_entry_t *d;
    HASH_REPLACE(hh, hash->h, k, sizeof(e->k), e, d);

    *out = (d == NULL) ? HASH_NOT_FOUND : d->v;
    free(d);

    return 0;
}

void
hash_free(hash_t *hash)
{
    hash_entry_t *curr, *tmp;

    HASH_ITER(hh, hash->h, curr, tmp) {
        HASH_DEL(hash->h, curr);
        free(curr);
    }

    free(hash);
}
