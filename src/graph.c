#include "../include/graph.h"

#include <stdio.h>
#include <stdlib.h>

// Creates the empty graph only using the size
Graph* create_graph(int size) {
    Graph* graph = malloc(sizeof(Graph));

    graph->size = size;
    graph->adjacents = malloc(size * sizeof(Edge*));
    for (int i = 0; i < size; i++) {
        graph->adjacents[i] = NULL;
    }

    return graph;
}

// Inserts a new edge in the graph
void create_edge(Graph* graph, NodeId origin, NodeId destination, CostUnit cost) {
    Edge* edge = malloc(sizeof(Edge));
    edge->destination = destination;
    edge->cost = cost;

    edge->next = graph->adjacents[origin];
    graph->adjacents[origin] = edge;
}

// Displays the current nodes and edges of the graph
void print_graph(Graph* graph) {
    for (int i = 1; i <= graph->size; i++) {
        printf("%d: ", i);

        Edge* edge = graph->adjacents[i - 1];
        while (edge != NULL) {
            printf("-> (%d | %d)", edge->destination + 1, edge->cost);
            edge = edge->next;
        }

        printf("\n");
    }
}

// Frees the allocated memory for the entire graph
void free_graph(Graph* graph) {
    for (int i = 0; i < graph->size; i++) {
        Edge* edge = graph->adjacents[i];
        while (edge != NULL) {
            Edge* temp = edge;
            edge = edge->next;
            free(temp);
        }
    }

    free(graph->adjacents);
    free(graph);
}