#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <n> <n_cpu> <seed> <lambda> <threshold>\n", *argv);
        return EXIT_FAILURE;
    }

    int n = atoi(*(argv + 1)); // number of processes to simulate
    int n_cpu = atoi(*(argv + 2)); // number of processes cpu bound
    int seed = atoi(*(argv + 3)); // pseduo-random number sequence seed
    double lambda = atof(*(argv + 4)); // interarrival times
    int threshold = atoi(*(argv + 5)); // upper bound

    printf("n = %d\n", n);
    printf("n_cpu = %d\n", n_cpu);
    printf("seed = %d\n", seed);
    printf("lambda = %f\n", lambda);
    printf("threshold = %d\n", threshold);

    // Check if n, n_cpu, seed, threshold are integers
    if (n != static_cast<int>(n) || n_cpu != static_cast<int>(n_cpu) || seed != static_cast<int>(seed) || threshold != static_cast<int>(threshold)) {
        fprintf(stderr, "ERROR: Invalid argument(s)\n");
        return EXIT_FAILURE;
    }
    // Check if lambda is float
    if (lambda != static_cast<double>(lambda)) {
        fprintf(stderr, "ERROR: Invalid argument(s)\n");
        return EXIT_FAILURE;
    }
    if (n <= 0 || n_cpu <= 0 || seed <= 0 || lambda <= 0 || threshold <= 0) {
        fprintf(stderr, "ERROR: Invalid argument(s)\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}