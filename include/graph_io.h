#ifndef FILE_H
#define FILE_H

#include "./graph.h"

Graph* read_graph(char* input_path, int* K);
void write_results(char* output_path);
void throwError(char* type, char* message);

#endif