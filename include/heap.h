#ifndef HEAP_H
#define HEAP_H

#include "graph.h"

typedef struct {
    CostUnit* data;
    int size;
    int removed;
} Heap;

Heap* create_heap(int size);
void heap_push(Heap* heap, CostUnit element);
CostUnit heap_pop(Heap* heap);
void free_heap(Heap* heap);
void print_heap(Heap* heap);

#endif