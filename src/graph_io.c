#include "../include/graph_io.h"

#include <stdio.h>
#include <stdlib.h>

// Stops the program in case of I/O errors
void throwError(char* type, char* message) {
    printf("%s ERROR - %s\n", type, message);
    exit(EXIT_FAILURE);
}

// Reads the graph from the given input file
Graph* read_graph(char* input_path, int* K) {
    FILE* input_file = fopen(input_path, "r");
    if (input_file == NULL) throwError("INPUT", "Cannot read file");

    // First line contains: N = number of nodes, M = number of edges, K = paths to find
    int N, M;
    fscanf(input_file, "%d %d %d", &N, &M, K);

    Graph* graph = create_graph(N);

    // Next lines contains the edges: A = origin, B = destination, C = cost
    int A, B, C;
    for (int i = 0; i < M; i++) {
        fscanf(input_file, "%d %d %d", &A, &B, &C);
        create_edge(graph, A - 1, B - 1, C);
    }

    fclose(input_file);
    return graph;
}

// Writes results in the output file
void write_results(char* output_path) {
}