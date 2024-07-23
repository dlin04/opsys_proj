#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

using namespace std;

void print_start(int n, int n_cpu, int seed, double lambda, int threshold) {
    cout << "<<< PROJECT PART I" << endl;
    if (n_cpu > 1) {
        cout << "<<< -- process set (n=" << n << ") with " << n_cpu << " CPU-bound processes" << endl;
    } else {
        cout << "<<< -- process set (n=" << n << ") with " << n_cpu << " CPU-bound process" << endl;
    }
    cout << "<<< -- seed=" << seed << "; lambda=" << fixed << setprecision(6) << lambda << "; bound=" << threshold << endl;
}

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

    // Check if n, n_cpu, seed, threshold are integers
    if (n != static_cast<int>(n) || n_cpu != static_cast<int>(n_cpu) || seed != static_cast<int>(seed) || threshold != static_cast<int>(threshold)) {
        cerr << "ERROR: Invalid argument(s)" << endl;
        return EXIT_FAILURE;
    }
    // Check if lambda is float
    if (lambda != static_cast<double>(lambda)) {
        cerr << "ERROR: Invalid argument(s)" << endl;
        return EXIT_FAILURE;
    }
    if (n <= 0 || n_cpu <= 0 || seed <= 0 || lambda <= 0 || threshold <= 0) {
        cerr << "ERROR: Invalid argument(s)" << endl;
        return EXIT_FAILURE;
    }

    print_start(n, n_cpu, seed, lambda, threshold);

    return EXIT_SUCCESS;
}