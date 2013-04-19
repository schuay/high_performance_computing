#include "itree.h"

#include <stdlib.h>
#include <string.h>

struct __itree_iter_t {
    const itree_t *root;        /**< The root of the iterated tree. */
    const itree_t *stack[32];   /**< The node stack. Tree depth cannot exceed 32
                                     since keys are uint32_t. */
    int top;                    /**< The current top stack index. */
};


static void
itree_insert_internal(const uint32_t index,
                      itree_t **root);

uint32_t
itree_insert(const uint32_t index,
             itree_t **root)
{
    itree_insert_internal(index, root);
    return 0;
}

static void
itree_insert_internal(const uint32_t index,
                      itree_t **root)
{
    itree_t *droot = *root;

    /* New node. */
    if (droot == NULL) {
        droot = calloc(1, sizeof(itree_t));
        if (droot == NULL) {
            /* TODO: Error. */
        }
        droot->k1 = index;
        droot->k2 = index;
        *root = droot;
        return;
    }

    /* Already inserted. */
    if (droot->k1 <= index && index <= droot->k2) {
        /* TODO: Error. */
    }

    /* Descend into left or right subtree. */
    if (droot->k1 > index) {
        itree_insert_internal(index, &droot->l);
        return;
    } else if (index > droot->k2) {
        itree_insert_internal(index, &droot->r);
        return;
    }
}

void
itree_free(itree_t *root)
{
    if (root == NULL) {
        return;
    }

    itree_free(root->l);
    itree_free(root->r);

    free(root);
}

#define ITER_PUSH(iter, val) do { iter->stack[iter->top++] = val; } while(0);
#define ITER_POP(iter) (iter->stack[--iter->top])

itree_iter_t *
itree_iter_init(const itree_t *root)
{
    itree_iter_t *iter = malloc(sizeof(itree_iter_t));
    if (iter == NULL) {
        return NULL;
    }

    memset(iter, 0, sizeof(itree_iter_t));
    iter->root = root;

    const itree_t *n = root;
    do {
        ITER_PUSH(iter, n);
        n = n->l;
    } while (n != NULL);

    return iter;
}

const itree_t *
itree_iter_next(itree_iter_t *iter)
{
    if (iter->top == 0) {
        return NULL;
    }

    const itree_t *next = ITER_POP(iter);

    const itree_t *n = next->r;
    while (n != NULL) {
        ITER_PUSH(iter, n);
        n = n->l;
    }

    return next;
}

void
itree_iter_free(itree_iter_t *iter)
{
    free(iter);
}
