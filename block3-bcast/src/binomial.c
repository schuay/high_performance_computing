#include "binomial.h"

#include <stdio.h>
#include <stdlib.h>

#define TAG (42)

typedef struct __node_t {
    struct __node_t *parent;
    struct __node_t **children;
    int order;
    int index;
} node_t;

static node_t *
_node_create(const int n);

static node_t *
_node_create_subtree(node_t *parent,
                     const int order,
                     const int index);

static const node_t *
_node_find(const node_t *node,
           const int index);

static void
_node_free(node_t *node);

int
bcast_binomial(int *buffer,
               int count,
               int root,
               MPI_Comm comm)
{
    int ret;
    
    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    /* 0-based rank for easier tree calculations. */
    const int rank0 = (rank + size - root) % size;

    node_t *node = _node_create(size);
    if (node == NULL) {
        return -1;
    }

    const node_t *self = _node_find(node, rank0);
    if (self == NULL) {
        ret = -1;
        goto out;
    }

    const int parent = (self->parent == NULL) ? -1 : (root + self->parent->index) % size;
    if (rank != root) {
        ret = MPI_Recv(buffer, count, MPI_INT, parent, TAG, comm, MPI_STATUS_IGNORE);
        if (ret != MPI_SUCCESS) {
            ret = -1;
            goto out;
        }
    }

    for (int i = self->order - 1; i >= 0; i--) {
        if (self->children[i] == NULL || self->children[i]->index >= size) {
            continue;
        }

        const int child = (root + self->children[i]->index) % size;
        ret = MPI_Send(buffer, count, MPI_INT, child, TAG, comm);
        if (ret != MPI_SUCCESS) {
            ret = -1;
            goto out;
        }
    }

    ret = 0;
out:
    _node_free(node);

    return ret;
}

static node_t *
_node_create(const int n)
{
    int order = 0;
    for (int i = 1; i < n; i <<= 1) {
        order++;
    }

    return _node_create_subtree(NULL, order, 0);
}

static node_t *
_node_create_subtree(node_t *parent,
                     const int order,
                     const int index)
{
    node_t *self = malloc(sizeof(node_t));
    if (self == NULL) {
        return NULL;
    }

    self->parent = parent;
    self->order = order;
    self->index = index;

    self->children = calloc(order, sizeof(node_t *));
    if (self->children == NULL) {
        _node_free(self);
        return NULL;
    }

    int next_index = index + 1;
    for (int i = 0; i < order; i++) {
        self->children[i] = _node_create_subtree(self, i, next_index);
        if (self->children[i] == NULL) {
            _node_free(self);
            return NULL;
        }
        next_index += (1 << i);
    }

    return self;
}

static const node_t *
_node_find(const node_t *node,
           const int index)
{
    if (node == NULL) {
        return NULL;
    }

    if (index == node->index) {
        return node;
    }

    for (int i = node->order - 1; i >= 0; i--) {
        if (index >= node->children[i]->index) {
            return _node_find(node->children[i], index);
        }
    }

    return NULL;
}

static void
_node_free(node_t *node)
{
    if (node->children != NULL) {
        for (int i = 0; i < node->order; i++) {
            if (node->children[i] != NULL) {
                _node_free(node->children[i]);
            }
        }
        free(node->children);
    }
    free(node);
}
