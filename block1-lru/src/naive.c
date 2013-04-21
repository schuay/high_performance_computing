#include "naive.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct __llist_t {
    struct __llist_t *prev, *next;
    uint64_t v;
} llist_t;

typedef struct {
    llist_t *head;
} llist_head_t;

static int
llist_push(llist_head_t *l,
           const uint64_t v)
{
    llist_t *head = malloc(sizeof(llist_t));
    if (head == NULL) {
        return -1;
    }

    head->prev = NULL;
    head->next = l->head;
    head->v = v;

    if (l->head != NULL) {
        l->head->prev = head;
    }

    l->head = head;

    return 0;
}

static llist_t *
llist_remove(llist_head_t *l,
             const uint64_t v,
             uint64_t *depth)
{
    uint64_t d = 0;
    llist_t *i = l->head;
    while (i != NULL) {
        if (i->v == v) {
            break;
        }
        i = i->next;
        d++;
    }

    if (i != NULL) {
        if (i->prev != NULL) {
            i->prev->next = i->next;
        } else {
            l->head = i->next;
        }

        if (i->next != NULL) {
            i->next->prev = i->prev;
        }

        *depth = d;
    }

    return i;
}

static void
llist_free(llist_head_t *l)
{
    llist_t *i = l->head;
    while (i != NULL) {
        llist_t *j = i->next;
        free(i);
        i = j;
    }
}

int
naive_process_trace(const uint64_t *p,
                    const uint64_t n,
                    const naive_cb fn)
{
    int ret = 0;

    llist_head_t head;
    memset(&head, 0, sizeof(llist_head_t));

    for (uint64_t i = 0; i < n; i++) {
        const uint64_t addr = p[i];

        uint64_t depth;
        llist_t *prev = llist_remove(&head, addr, &depth);
        free(prev);

        if (llist_push(&head, addr) != 0) {
            ret = -1;
            goto out;
        }

        fn(i, (prev == NULL) ? NAIVE_INFINITE : depth);
    }

out:
    llist_free(&head);

    return ret;
}
