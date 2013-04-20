#include "itree.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* itree defines. */

#define MAX(l, r) (((l) > (r)) ? (l) : (r))
#define MAX_H(l, r) MAX((l == NULL) ? 0 : l->h, (r == NULL) ? 0 : r->h)


/* itree structs. */

struct __itree_iter_t {
    const itree_t *root;        /**< The root of the iterated tree. */
    const itree_t *stack[32];   /**< The node stack. Tree depth cannot exceed 32
                                     since keys are uint32_t. */
    int top;                    /**< The current top stack index. */
};

typedef struct {
    itree_t *u, *l;             /**< Upper and lower adjacent nodes. */
} itree_util_t;


/* itree declarations. */

static int
_itree_insert(const uint32_t index,
              itree_t **root,
              uint32_t *holes,
              itree_util_t *util);
static int
_itree_new_node(const uint32_t index,
                      itree_t **root);
static void
_itree_extend_node(const uint32_t index,
                   itree_t *node);
static void
_itree_merge_nodes(itree_t *upper,
                   itree_t *lower);
static int
_itree_descend_l(const uint32_t index,
                 itree_t **root,
                 uint32_t *holes,
                 itree_util_t *util);
static int
_itree_descend_r(const uint32_t index,
                 itree_t **root,
                 uint32_t *holes,
                 itree_util_t *util);

/* itree definitions. */

int
itree_insert(const uint32_t index,
                      itree_t **root,
                      uint32_t *holes)
{
    itree_util_t util;
    memset(&util, 0, sizeof(itree_util_t));

    *holes = 0;

    int ret = _itree_insert(index, root, holes, &util);

    if (util.l != NULL) {
        free(util.l);
    }

    return ret;
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

/**
 * Extends node by adding index to the node interval.
 *
 * Preconditions:
 *  * node != NULL.
 *  * index is immediately adjacent to the node interval.
 *
 * Postconditions:
 *  * The node interval has been extended by index.
 */
static void
_itree_extend_node(const uint32_t index,
                   itree_t *node)
{
    assert(index == node->k1 - 1 || node->k2 + 1 == index);

    if (index < node->k1) {
        node->k1 = index;
    } else {
        node->k2 = index;
    }
}

/**
 * Merges lower into upper. Lower is *not* deleted.
 *
 * Preconditions:
 *  * lower, upper != NULL.
 *  * upper->k2 + 1 == lower->k1 - 1 OR
 *    upper->k1 - 1 == lower->k2 + 1
 *
 * Postconditions:
 *  * The nodes are merged.
 */
static void
_itree_merge_nodes(itree_t *upper,
                   itree_t *lower)
{
    assert(upper->k2 + 1 == lower->k1 - 1 || upper->k1 - 1 == lower->k2 + 1);

    if (upper->k1 > lower->k2) {
        upper->k1 = lower->k1;
    } else {
        upper->k2 = lower->k2;
    }
}

static int
_itree_descend_l(const uint32_t index,
                 itree_t **root,
                 uint32_t *holes,
                 itree_util_t *util)
{
    itree_t *droot = *root;

    *holes += droot->v + droot->k2 - droot->k1 + 1;

    if (droot->k1 == index + 1) {
        if (util->u == NULL) {
            util->u = droot;
        } else {
            util->l = droot;
        }
    }
    int ret = _itree_insert(index, &droot->l, holes, util);
    if (ret != 0) { return ret; }

    /* Remove the lower node. */
    if (util->l != NULL && util->l == droot->l) {
        droot->l = util->l->r;
    }

    droot->h = MAX_H(droot->l, droot->r) + 1;

    return 0;
}

static int
_itree_descend_r(const uint32_t index,
                 itree_t **root,
                 uint32_t *holes,
                 itree_util_t *util)
{
    itree_t *droot = *root;

    if (droot->k2 == index - 1) {
        if (util->u == NULL) {
            util->u = droot;
        } else {
            util->l = droot;
        }
    }

    /* Index was added as a new descendant node. */
    if (util->u == NULL && util->u != droot) {
        droot->v++;
    }

    int ret = _itree_insert(index, &droot->r, holes, util);
    if (ret != 0) { return ret; }

    /* Remove the lower node. */
    if (util->l != NULL && util->l == droot->r) {
        droot->r = util->l->l;
    }

    /* Adjust the subtree sum after a merge. */
    if (util->l != NULL && util->l != droot) {
        droot->v -= util->l->v + util->l->k2 - util->l->k1 + 1;
    }

    droot->h = MAX_H(droot->l, droot->r) + 1;

    return 0;
}

/**
 * The workhorse for itree_insert.
 * Util keeps track of several internal variables needed for merging nodes.
 */
static int
_itree_insert(const uint32_t index,
                      itree_t **root,
                      uint32_t *holes,
                      itree_util_t *util)
{
    itree_t *droot = *root;

    /* Merge two existing nodes. */
    if (droot == NULL && util->l != NULL) {
        _itree_merge_nodes(util->u, util->l);
        return 0;
    }

    /* Add to existing adjacent node. */
    if (droot == NULL && util->u != NULL) {
        _itree_extend_node(index, util->u);
        return 0;
    }

    /* New node. */
    if (droot == NULL) {
        return _itree_new_node(index, root);
    }

    /* Descend into left or right subtree. */
    if (droot->k1 > index) {
        return _itree_descend_l(index, root, holes, util);
    } else if (index > droot->k2) {
        return _itree_descend_r(index, root, holes, util);
    } else {
        fprintf(stderr, "Index %d is already in tree\n", index);
        return -1;
    }

    return 0;
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


/* itree_iter defines. */

#define ITER_PUSH(iter, val) do { iter->stack[iter->top++] = val; } while(0);
#define ITER_POP(iter) (iter->stack[--iter->top])


/* itree_iter definitions. */

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
