#ifndef __ITREE_H
#define __ITREE_H

/**
 * An AVL tree with closed, mutually disjunct uint32_t intervals as keys and
 * uint32_t values representing the index count in the right subtree (the
 * interval [5, 7] counts as 3 indices).
 */

typedef struct {
    itree_t *l, *r;         /**< The left and right child nodes. */
    uint32_t k1, k2;        /**< The key interval [k1, k2]. */
    uint32_t v;             /**< The # of elements in the right subtree. */
} itree_t;

/**
 * Inserts a new index into the tree and returns the # of indices in the
 * tree larger than the new index.
 */
uint32_t
itree_insert(const uint32_t index,
             itree_t **root);

#endif /*  __ITREE_H */
