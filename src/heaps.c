#include "../include/heaps.h"

#include <stdio.h>
#include <stdlib.h>

/* ========== LEFTIST HEAP ==========
 * Persistent heap optimized for fast merges
 * Used to store sidetracks by its delta value
 *
 * Free all HeapNode structures created is a hard task
 * A same pointer can be used in multiple heaps simultaneously
 * To avoid memory leaks, all created nodes are registered in an array
 * ================================== */

HeapNode** all_nodes = NULL;
int node_count = 0;
int node_capacity = 0;

void freeAllHeapNodes() {
    for (int i = 0; i < node_count; i++) {
        free(all_nodes[i]);
    }
    free(all_nodes);
}

void register_node(HeapNode* node) {
    if (node_count == node_capacity) {
        node_capacity = node_capacity ? node_capacity * 2 : 1024;
        all_nodes = realloc(all_nodes, node_capacity * sizeof(HeapNode*));
    }
    all_nodes[node_count++] = node;
}

HeapNode* create_heap_node(Sidetrack s) {
    HeapNode* h = malloc(sizeof(HeapNode));
    h->s = s;
    h->left = h->right = NULL;
    h->rank = 1;
    register_node(h);
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
    register_node(new);

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

/* ========== BINARY HEAP ==========
 * Used to store PathNode structures by its cost value
 * The smallest-cost path is always at the root
 * ================================= */

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

void freeBinaryHeap(BinaryHeap* heap) {
    if (!heap) return;
    free(heap->data);
    free(heap);
}