#ifndef GRAṔH_H
#define GRAṔH_H

typedef int NodeId;
typedef int CostUnit;

typedef struct Edge {
    NodeId destination;
    CostUnit cost;
    struct Edge* next;
} Edge;

typedef struct {
    int size;
    Edge** adjacents;
} Graph;

Graph* create_graph(int size);
void create_edge(Graph* graph, NodeId origin, NodeId destination, CostUnit cost);
void print_graph(Graph* graph);
void free_graph(Graph* graph);

#endif