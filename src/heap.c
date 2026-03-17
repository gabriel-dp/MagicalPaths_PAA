#include "../include/heap.h"

#include <stdio.h>
#include <stdlib.h>

// Create and initialize heap
Heap* create_heap(int size) {
    Heap* heap = malloc(sizeof(Heap));
    heap->size = 0;
    heap->removed = 0;
    heap->data = malloc(sizeof(CostUnit) * size);
    return heap;
}

// Push operation inserts the element in the sorted position
void heap_push(Heap* h, CostUnit element) {
    int i = h->size++;

    h->data[i] = element;

    while (i > 0) {
        int parent = (i - 1) / 2;

        if (h->data[parent] <= h->data[i])
            break;

        CostUnit tmp = h->data[parent];
        h->data[parent] = h->data[i];
        h->data[i] = tmp;

        i = parent;
    }
}

// Pop operation removes the first element
// No insertions must be done after popping out an element!!
CostUnit heap_pop(Heap* heap) {
    CostUnit result = heap->data[heap->removed++];
    heap->size--;
    return result;
}

// Free alocated memory for heap
void free_heap(Heap* heap) {
    free(heap->data);
    free(heap);
}

// Display heap elements
void print_heap(Heap* heap) {
    for (int i = 0; i < heap->size; i++) {
        printf("| %d ", heap->data[i]);
    }
    printf("|\n");
}