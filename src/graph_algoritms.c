#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/graph_algorithms.h"
#include "../include/heap.h"

// Creates a new graph swapping all origins and destinations from edges
Graph* reverse_graph(Graph* graph) {
    Graph* reverse = create_graph(graph->size);
    for (int v = 0; v < graph->size; v++) {
        Edge* edge = graph->adjacents[v];
        while (edge != NULL) {
            create_edge(reverse, edge->destination, v, edge->cost);
            edge = edge->next;
        }
    }
    return reverse;
}

// Dijkstra algorithm, gets the minimum distance from a source to all other nodes
CostUnit* dijkstra(Graph* graph, NodeId source, NodeId parent[]) {
    CostUnit* distances = malloc(sizeof(CostUnit) * graph->size);
    int visited[graph->size];

    for (int i = 0; i < graph->size; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }

    distances[source] = 0;

    for (int i = 0; i < graph->size; i++) {
        int u = -1;
        CostUnit min = INT_MAX;
        for (int v = 0; v < graph->size; v++) {
            if (!visited[v] && distances[v] < min) {
                min = distances[v];
                u = v;
            }
        }
        if (u == -1) break;
        visited[u] = 1;

        Edge* e = graph->adjacents[u];
        while (e) {
            int v = e->destination;
            CostUnit w = e->cost;
            if (!visited[v] && distances[u] != INT_MAX && distances[u] + w < distances[v]) {
                distances[v] = distances[u] + w;
                parent[v] = u;
            }
            e = e->next;
        }
    }

    return distances;
}

// Build tree so it is possible to propagate the heaps
Graph* build_tree(NodeId* parent, int n) {
    Graph* tree = create_graph(n);
    for (int v = 0; v < n; v++) {
        if (parent[v] != -1) {
            create_edge(tree, parent[v], v, 0);
        }
    }
    return tree;
}

// Propagate heaps so every heap will contain its "childs"
void propagate_heaps(NodeId u, Graph* tree, HeapNode** H) {
    for (Edge* e = tree->adjacents[u]; e; e = e->next) {
        NodeId v = e->destination;
        H[v] = heap_merge(H[v], H[u]);
        propagate_heaps(v, tree, H);
    }
}

// Generate a heap for each node based on the possible sidetrack edges
HeapNode** build_sidetrack_heaps(Graph* g, CostUnit* dist, NodeId* parent, NodeId destination) {
    HeapNode** H = calloc(g->size, sizeof(HeapNode*));

    // Identify all sidetrack edges
    for (NodeId u = 0; u < g->size; u++) {
        for (Edge* e = g->adjacents[u]; e; e = e->next) {
            NodeId v = e->destination;
            if (parent[u] == v || dist[v] == INT_MAX) continue;  // skip shortest-path-tree edge

            CostUnit delta = e->cost + dist[v] - dist[u];
            Sidetrack s = {u, v, delta};
            H[u] = heap_merge(H[u], create_heap_node(s));
        }
    }

    // Propagate heaps so every heap will include its childs
    Graph* tree = build_tree(parent, g->size);
    propagate_heaps(destination, tree, H);

    free_graph(tree);

    return H;
}

// Eppstein algorithm, returns the K min paths from a source to a destination
CostUnit* eppstein(Graph* graph, int K, NodeId source, NodeId destination) {
    // Gets minimum distances from every node to destination using the reverse graph
    Graph* reverse = reverse_graph(graph);
    NodeId parent[graph->size];  // shortest-path-tree
    for (int i = 0; i < graph->size; i++) {
        parent[i] = -1;
    }
    CostUnit* distances_to_destination = dijkstra(reverse, destination, parent);

    // Generates a heap for each node based on the possible sidetrack edges
    HeapNode** H = build_sidetrack_heaps(graph, distances_to_destination, parent, destination);

    // Creates heap of heaps to store possible paths
    BinaryHeap* pq = create_binary_heap(K * 10);
    if (H[source]) {
        binary_heap_push_path(pq, (PathNode){distances_to_destination[source] + H[source]->s.delta, H[source]});
    }

    // Retrieve K min paths
    int count = 0;
    CostUnit* min_paths = calloc(K, sizeof(CostUnit));
    min_paths[count++] = distances_to_destination[source];
    while (count < K && pq->size > 0) {
        // Extract best path from heap
        PathNode cur = binary_heap_pop_path(pq);
        min_paths[count++] = cur.cost;
        HeapNode* h = cur.heap;

        // Expand simbling sidetracks
        if (h->left) binary_heap_push_path(pq, (PathNode){cur.cost - h->s.delta + h->left->s.delta, h->left});
        if (h->right) binary_heap_push_path(pq, (PathNode){cur.cost - h->s.delta + h->right->s.delta, h->right});

        // Expand new possible paths from current path
        NodeId v = h->s.to;
        if (H[v]) {
            binary_heap_push_path(pq, (PathNode){cur.cost + H[v]->s.delta, H[v]});
        }
    }

    return min_paths;
}