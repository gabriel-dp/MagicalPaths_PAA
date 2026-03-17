#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include "./graph.h"

Graph* reverse_graph(Graph* graph);
CostUnit* dijkstra(Graph* graph, NodeId source, NodeId parent[]);
CostUnit* eppstein(Graph* graph, int K, NodeId source, NodeId destination);

#endif