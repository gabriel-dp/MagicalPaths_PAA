#ifndef TIME_H
#define TIME_H

typedef long double Time;

Time get_real_time();
Time get_cpu_time();
Time format_time(long int sec, long int usec);
void print_elapsed_time(char* message, Time start, Time end);

#endif