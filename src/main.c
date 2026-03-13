#include <stdio.h>
#include <stdlib.h>

#include "../include/args.h"
#include "../include/time.h"

int main(int argc, char* argv[]) {
    Time start_cpu = get_cpu_time();
    Time start_real = get_real_time();

    char *input_path, *output_path;
    get_args(&input_path, &output_path, argc, argv);
    printf("INPUT : %s\nOUTPUT: %s\n", input_path, output_path);

    Time end_cpu = get_cpu_time();
    Time end_real = get_real_time();

    print_elapsed_time("CPU ", start_cpu, end_cpu);
    print_elapsed_time("Real", start_real, end_real);

    return 0;
}