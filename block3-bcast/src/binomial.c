#include "binomial.h"

#include <stdlib.h>

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

static void
_node_free(node_t *node);

int
bcast_binomial(int *buffer,
               int count,
               int root,
               MPI_Comm comm)
{
    node_t *node = _node_create(count);
    _node_free(node);

    return -1;
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
