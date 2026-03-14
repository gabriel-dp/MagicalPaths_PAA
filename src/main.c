#include <stdio.h>
#include <stdlib.h>

#include "../include/args.h"
#include "../include/graph.h"
#include "../include/graph_io.h"
#include "../include/time.h"

int main(int argc, char* argv[]) {
    Time start_cpu = get_cpu_time(), start_real = get_real_time();

    char *input_path, *output_path;
    get_args(&input_path, &output_path, argc, argv);

    int K;
    Graph* graph = read_graph(input_path, &K);
    print_graph(graph);

    Time end_cpu = get_cpu_time(), end_real = get_real_time();

    print_elapsed_time("CPU ", start_cpu, end_cpu);
    print_elapsed_time("Real", start_real, end_real);

    free_graph(graph);
    return 0;
}