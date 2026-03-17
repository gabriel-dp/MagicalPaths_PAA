#include <stdio.h>
#include <stdlib.h>

#include "../include/args.h"
#include "../include/graph.h"
#include "../include/graph_algorithms.h"
#include "../include/graph_io.h"
#include "../include/time.h"

int main(int argc, char* argv[]) {
    // Gets the input and output paths from args
    char *input_path, *output_path;
    get_args(&input_path, &output_path, argc, argv);

    // Receives graph and K from input
    int K;
    Graph* graph = read_graph(input_path, &K);
    print_graph(graph);

    // Runs algorithm measuring time
    Time start_real = get_real_time(), start_cpu = get_cpu_time();
    CostUnit* min_paths = eppstein(graph, K, 0, graph->size - 1);
    Time end_cpu = get_cpu_time(), end_real = get_real_time();

    // Registers results to output
    print_elapsed_time("CPU ", start_cpu, end_cpu);
    print_elapsed_time("Real", start_real, end_real);
    write_results(output_path, min_paths, K);

    // Frees allocated memmory
    free(min_paths);
    free_graph(graph);

    return 0;
}