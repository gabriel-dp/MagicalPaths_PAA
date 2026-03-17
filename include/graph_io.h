#ifndef GRAPH_IO_H
#define GRAPH_IO_H

#include "./graph.h"

Graph* read_graph(char* input_path, int* K);
void write_results(char* output_path, CostUnit* results, int K);
void throwError(char* type, char* message);

#endif