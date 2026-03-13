#include "../include/time.h"

#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>

// Returns real time in that moment
Time get_real_time() {
    struct timeval now;
    gettimeofday(&now, NULL);
    return format_time(now.tv_sec, now.tv_usec);
}

// Returns CPU time in that moment
Time get_cpu_time() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return format_time(usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
}

// Join time in seconds and microseconds into a long double variable
Time format_time(long int sec, long int usec) {
    Time total_time = sec + ((Time)usec / 1000000.0L);
    return total_time;
}

// Prints the difference between start to end
void print_elapsed_time(char* message, Time start, Time end) {
    Time elapsed_time = end - start;
    printf("%s = %Lf\n", message, elapsed_time);
}