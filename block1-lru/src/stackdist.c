#include "stackdist.h"

#include <stddef.h>

#include "hash.h"
#include "itree.h"

int
stackdist_process_trace(const uint64_t *p,
                        const uint64_t n,
                        const stackdist_cb fn)
{
    int ret = 0;
    hash_t *hash = NULL;
    itree_t *itree = NULL;
    
    hash = hash_init();
    if (hash == NULL) {
        goto out;
    }

    for(uint64_t i = 0; i < n; i++) {
        const uint64_t addr = p[i];

        /* Search. */

        uint64_t last;
        if ((ret = hash_insert(hash, addr, i, &last)) != 0) {
            goto out;
        }

        if (last == HASH_NOT_FOUND) {
            fn(i, STACKDIST_INFINITE);
            continue;
        }
        
        uint64_t holes;
        if ((ret = itree_insert(last, &itree, &holes)) != 0) {
            goto out;
        }

        const uint64_t dist = i - last - holes - 1;
        fn(i, dist);
    }
out:
    if (hash != NULL) {
        hash_free(hash);
    }

    if (itree != NULL) {
        itree_free(itree);
    }

    return ret;
}
