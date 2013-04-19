#include "itree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX(l, r) (((l) > (r)) ? (l) : (r))
#define MAX_H(l, r) MAX((l == NULL) ? 0 : l->h, (r == NULL) ? 0 : r->h)

struct __itree_iter_t {
    const itree_t *root;        /**< The root of the iterated tree. */
    const itree_t *stack[32];   /**< The node stack. Tree depth cannot exceed 32
                                     since keys are uint32_t. */
    int top;                    /**< The current top stack index. */
};


static int
_itree_insert(const uint32_t index,
              itree_t **root,
              uint32_t *holes);
static int
_itree_new_node(const uint32_t index,
                      itree_t **root);

int
itree_insert(const uint32_t index,
                      itree_t **root,
                      uint32_t *holes)
{
    return _itree_insert(index, root, holes);
}

static int
_itree_new_node(const uint32_t index,
                      itree_t **root)
{
    itree_t *droot = calloc(1, sizeof(itree_t));
    if (droot == NULL) {
        perror("calloc");
        return -1;
    }
    droot->k1 = index;
    droot->k2 = index;
    *root = droot;
    return 0;
}

static int
_itree_insert(const uint32_t index,
                      itree_t **root,
                      uint32_t *holes)
{
    int ret = 0;
    itree_t *droot = *root;

    /* New node. */
    if (droot == NULL) {
        *holes = 0;
        return _itree_new_node(index, root);
    }

    /* Descend into left or right subtree. */
    if (droot->k1 > index) {
        ret = _itree_insert(index, &droot->l, holes);
        if (ret != 0) { return ret; }

        *holes += droot->v + droot->k2 - droot->k1 + 1;
        droot->h = MAX_H(droot->l, droot->r) + 1;
    } else if (index > droot->k2) {
        droot->v++;
        ret = _itree_insert(index, &droot->r, holes);
        if (ret != 0) { return ret; }

        droot->h = MAX_H(droot->l, droot->r) + 1;
    } else {
        fprintf(stderr, "Index %d is already in tree\n", index);
        return -1;
    }

    return ret;
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
