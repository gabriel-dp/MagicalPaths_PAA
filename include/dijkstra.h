#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

typedef struct {
    int node;
    CostUnit dist;
} MinHeapNode;

typedef struct {
    MinHeapNode* data;
    int size;
    int capacity;
} MinHeap;

CostUnit* dijkstra(Graph* graph, NodeId source, NodeId parent[]);

#endif