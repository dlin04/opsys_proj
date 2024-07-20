#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    int n = atoi(*(argv + 1)); // number of processes to simulate
    int n_cpu = atoi(*(argv + 2)); // number of processes cpu bound
    int seed = atoi(*(argv + 3)); // pseduo-random number sequence seed
    int lambda = atoi(*(argv + 4)); // interarrival times
    int threshold = atoi(*(argv + 5)); // upper bound

    return EXIT_SUCCESS;
}