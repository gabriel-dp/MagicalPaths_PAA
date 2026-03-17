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

// Creates a min heap with all sidetrack edges
Heap* build_sidetracks_heap(Graph* graph, CostUnit* distances_to_destination, NodeId source, NodeId* parent) {
    Heap* min_heap = create_heap(graph->edges_count);

    for (int u = 0; u < graph->size; u++) {
        Edge* e = graph->adjacents[u];

        while (e) {
            int v = e->destination;
            CostUnit w = e->cost;

            // Skips the edges used in the shortest path tree
            if (parent[u] == v) {
                e = e->next;
                continue;
            }

            // Inserts the cost of the new path using the sidetrack edge in the min heap
            CostUnit delta = w + distances_to_destination[v] - distances_to_destination[u];
            heap_push(min_heap, distances_to_destination[source] + delta);

            e = e->next;
        }
    }

    return min_heap;
}

// Eppstein algorithm, returns the K min paths from a source to a destination
CostUnit* eppstein(Graph* graph, int K, NodeId source, NodeId destination) {
    // Gets minimum distances from every node to destination using the reverse graph
    Graph* reverse = reverse_graph(graph);
    NodeId parent[graph->size];  // Minimum Spanning Tree
    for (int i = 0; i < graph->size; i++) {
        parent[i] = -1;
    }
    CostUnit* distances_to_destination = dijkstra(reverse, destination, parent);

    // Builds a min heap with the identified sidetrack edges
    Heap* sidetracks_heap = build_sidetracks_heap(graph, distances_to_destination, source, parent);

    // Results will be the array of the best K paths sorted by cost
    CostUnit* min_paths = calloc(K, sizeof(CostUnit));
    min_paths[0] = distances_to_destination[source];

    // Pops out K paths from min heap
    for (int i = 1; i < K && sidetracks_heap->size > 0; i++) {
        min_paths[i] = heap_pop(sidetracks_heap);
    }

    // Frees allocated memory during algorithm execution
    free_heap(sidetracks_heap);
    free(distances_to_destination);
    free_graph(reverse);

    return min_paths;
}