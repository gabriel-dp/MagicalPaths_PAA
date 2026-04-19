#include "../include/eppstein.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "../include/dijkstra.h"
#include "../include/heaps.h"
#include "../include/time.h"

// Build tree using a NodeId parent array
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
HeapNode** build_sidetrack_heaps(Graph* graph, CostUnit* distances_to_destination, NodeId* parent, NodeId destination) {
    HeapNode** H = calloc(graph->size, sizeof(HeapNode*));

    // Identify all sidetrack edges
    for (NodeId u = 0; u < graph->size; u++) {
        for (Edge* e = graph->adjacents[u]; e; e = e->next) {
            NodeId v = e->destination;
            if (parent[u] == v || distances_to_destination[v] == INT_MAX) continue;  // skip shortest-path-tree edge

            CostUnit delta = e->cost + distances_to_destination[v] - distances_to_destination[u];
            Sidetrack s = {u, v, delta};
            H[u] = heap_merge(H[u], create_heap_node(s));
        }
    }

    // Propagate heaps so every heap will include its childs
    Graph* tree = build_tree(parent, graph->size);
    propagate_heaps(destination, tree, H);
    free_graph(tree);

    return H;
}

// Eppstein algorithm, returns the K min paths from a source to a destination
CostUnit* eppstein(Graph* graph, int K, NodeId source, NodeId destination) {
    // STEP 1 (Pre-processing)
    // Obtain minimum distances from every node to destination using the reverse graph
    Time start_1 = get_cpu_time();
    Graph* reverse = reverse_graph(graph);
    NodeId* parent = malloc(graph->size * sizeof(NodeId));
    memset(parent, -1, graph->size * sizeof(NodeId));
    // Use Dijkstra in the reverse graph to get the minimum distances from all nodes to destination
    CostUnit* distances_to_destination = dijkstra(reverse, destination, parent);
    Time end_1 = get_cpu_time();

    // STEP 2 (Building heaps)
    // Generate leftist heap for sidetrack per node and a binary heap for paths
    Time start_2 = get_cpu_time();
    HeapNode** H = build_sidetrack_heaps(graph, distances_to_destination, parent, destination);
    BinaryHeap* pq = create_binary_heap(K * 10);
    binary_heap_push_path(pq, (PathNode){distances_to_destination[source] + H[source]->s.delta, H[source]});
    Time end_2 = get_cpu_time();

    // STEP 3 (Extract and expand)
    // Retrieve K min paths from heaps
    Time start_3 = get_cpu_time();
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
    Time end_3 = get_cpu_time();

    // Print elased time per step
    print_elapsed_time("Pre-processing    ", start_1, end_1);
    print_elapsed_time("Building heaps    ", start_2, end_2);
    print_elapsed_time("Extract and expand", start_3, end_3);

    // Free all allocated memory
    free_graph(reverse);
    free(parent);
    free(distances_to_destination);
    freeBinaryHeap(pq);
    freeAllHeapNodes();
    free(H);

    return min_paths;
}