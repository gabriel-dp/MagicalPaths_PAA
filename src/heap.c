#include "../include/heap.h"

#include <stdio.h>
#include <stdlib.h>

/* === LEFTIST HEAP ==== */

HeapNode* create_heap_node(Sidetrack s) {
    HeapNode* h = malloc(sizeof(HeapNode));
    h->s = s;
    h->left = h->right = NULL;
    h->rank = 1;
    return h;
}

HeapNode* heap_merge(HeapNode* a, HeapNode* b) {
    if (!a) return b;
    if (!b) return a;

    if (a->s.delta > b->s.delta) {
        HeapNode* tmp = a;
        a = b;
        b = tmp;
    }

    HeapNode* new = malloc(sizeof(HeapNode));
    *new = *a;

    new->right = heap_merge(a->right, b);

    int left_rank = new->left ? new->left->rank : 0;
    int right_rank = new->right ? new->right->rank : 0;

    if (left_rank < right_rank) {
        HeapNode* tmp = new->left;
        new->left = new->right;
        new->right = tmp;
    }

    new->rank = (new->right ? new->right->rank : 0) + 1;

    return new;
}

/* === BINARY HEAP ==== */

BinaryHeap* create_binary_heap(int capacity) {
    BinaryHeap* h = malloc(sizeof(BinaryHeap));
    h->data = malloc(sizeof(PathNode) * capacity);
    h->size = 0;
    h->capacity = capacity;
    return h;
}

void swap_path(PathNode* a, PathNode* b) {
    PathNode tmp = *a;
    *a = *b;
    *b = tmp;
}

void binary_heap_push_path(BinaryHeap* h, PathNode value) {
    if (h->size == h->capacity) {
        h->capacity *= 2;
        h->data = realloc(h->data, sizeof(PathNode) * h->capacity);
    }

    int i = h->size++;
    h->data[i] = value;

    while (i > 0) {
        int parent = (i - 1) / 2;

        if (h->data[parent].cost <= h->data[i].cost)
            break;

        swap_path(&h->data[parent], &h->data[i]);
        i = parent;
    }
}

PathNode binary_heap_pop_path(BinaryHeap* h) {
    PathNode result = h->data[0];

    h->data[0] = h->data[--h->size];

    int i = 0;

    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < h->size && h->data[left].cost < h->data[smallest].cost)
            smallest = left;

        if (right < h->size && h->data[right].cost < h->data[smallest].cost)
            smallest = right;

        if (smallest == i)
            break;

        swap_path(&h->data[i], &h->data[smallest]);
        i = smallest;
    }

    return result;
}

void print_pathnode(PathNode p) {
    if (!p.heap) {
        printf("[cost=%d, heap=NULL]\n", p.cost);
        return;
    }
    printf("[cost=%d | delta=%d | edge=%d->%d | heap=%p]\n", p.cost, p.heap->s.delta, p.heap->s.from + 1, p.heap->s.to + 1, (void*)p.heap);
}

void print_binary_heap(BinaryHeap* h) {
    printf("\n=== BINARY HEAP (size=%d) ===\n", h->size);
    for (int i = 0; i < h->size; i++) {
        printf("i=%d ", i);
        print_pathnode(h->data[i]);
    }
    printf("============================\n");
}