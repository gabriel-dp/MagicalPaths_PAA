#ifndef HEAP_H
#define HEAP_H

#include "graph.h"

typedef struct HeapNode {
    Sidetrack s;
    struct HeapNode *left, *right;
    int rank;
} HeapNode;

typedef struct {
    CostUnit cost;
    HeapNode* heap;
} PathNode;

typedef struct {
    PathNode* data;
    int size;
    int capacity;
} BinaryHeap;

HeapNode* create_heap_node(Sidetrack s);
HeapNode* heap_merge(HeapNode* a, HeapNode* b);

BinaryHeap* create_binary_heap(int capacity);
void binary_heap_push_path(BinaryHeap* h, PathNode value);
PathNode binary_heap_pop_path(BinaryHeap* h);

void print_pathnode(PathNode p);
void print_binary_heap(BinaryHeap* h);

#endif