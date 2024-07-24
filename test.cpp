#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <random>
#include <algorithm> // Add this line to include the algorithm header

using namespace std;

class Process {
public:
    string id;
    int arrivalTime;
    vector<int> cpuBursts;
    vector<int> ioBursts;

    Process(string id, int arrivalTime) : id(id), arrivalTime(arrivalTime) {}
};

double next_exp(double lambda, mt19937_64& rng) {
    uniform_real_distribution<double> dist(0.0, 1.0);
    return -log(dist(rng)) / lambda;
}

vector<Process> generateProcesses(int n, int ncpu, double lambda, int bound, int seed) {
    vector<Process> processes;
    mt19937_64 rng(seed);

    auto generateBurstTime = [&](double factor) {
        double burst;
        do {
            burst = next_exp(lambda, rng);
        } while (burst > bound);
        return static_cast<int>(ceil(burst * factor));
    };

    for (int i = 0; i < n; ++i) {
        string id = string(1, 'A' + i / 10) + to_string(i % 10);
        int arrivalTime = static_cast<int>(floor(next_exp(lambda, rng)));
        Process process(id, arrivalTime);

        int numBursts = static_cast<int>(ceil(drand48() * 32));
        bool isCpuBound = i < ncpu;

        for (int j = 0; j < numBursts; ++j) {
            double cpuFactor = isCpuBound ? 4.0 : 1.0;
            double ioFactor = isCpuBound ? 1.0 / 8.0 : 8.0;

            process.cpuBursts.push_back(generateBurstTime(cpuFactor));
            if (j < numBursts - 1) {
                process.ioBursts.push_back(generateBurstTime(ioFactor));
            }
        }

        processes.push_back(process);
    }

    return processes;
}

void printProcesses(const vector<Process>& processes, int ncpu, int seed, double lambda, int bound) {
    cout << "<<< PROJECT PART I" << endl;
    cout << "<<< -- process set (n=" << processes.size() << ") with " << ncpu << " CPU-bound process(es)" << endl;
    cout << "<<< -- seed=" << seed << "; lambda=" << lambda << "; bound=" << bound << endl;

    for (const auto& process : processes) {
        cout << (find(process.id.begin(), process.id.end(), '0') != process.id.end() ? "CPU-bound" : "I/O-bound")
             << " process " << process.id << ": arrival time " << process.arrivalTime << "ms; "
             << process.cpuBursts.size() << " CPU bursts:" << endl;

        for (size_t i = 0; i < process.cpuBursts.size(); ++i) {
            cout << "==> CPU burst " << process.cpuBursts[i] << "ms";
            if (i < process.ioBursts.size()) {
                cout << " ==> I/O burst " << process.ioBursts[i] << "ms";
            }
            cout << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        cerr << "Usage: " << argv[0] << " <n> <ncpu> <seed> <lambda> <bound>" << endl;
        return 1;
    }

    int n = stoi(argv[1]);
    int ncpu = stoi(argv[2]);
    int seed = stoi(argv[3]);
    double lambda = stod(argv[4]);
    int bound = stoi(argv[5]);

    srand48(seed);

    auto processes = generateProcesses(n, ncpu, lambda, bound, seed);
    printProcesses(processes, ncpu, seed, lambda, bound);

    return 0;
}