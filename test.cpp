#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <random>
#include <algorithm> // Add this line to include the algorithm header

using namespace std;

class Process
{
public:
    string id;
    int arrivalTime;
    vector<int> cpuBursts;
    vector<int> ioBursts;

    Process(string id, int arrivalTime) : id(id), arrivalTime(arrivalTime) {}
};

class ExponentialDistribution
{
private:
    int seed;
    double lambda;
    int max;

public:
    ExponentialDistribution(int seed, double lambda, int max)
    {
        this->seed = seed;
        this->lambda = lambda;
        this->max = max;
        srand48(this->seed);
    }

    double next_exp()
    {
        while(true) {
            double r = drand48();
            double num = -log(r) / lambda;
            if(num <= max) {
                return num;
            }
        }
    }
};

vector<Process> generateProcesses(int n, int ncpu, ExponentialDistribution ed)
{
    vector<Process> processes;
    char letter = 'A';
    int number = 0;
    // Loop over process count
    for (int i = 0; i < n; i++)
    {
        // Get random number
        int arrival = floor(ed.next_exp());
        // Get the process ID
        if (number > 9)
        {
            number = 0;
            letter++;
        }
        char num = '0' + number;
        string id = string(1, letter) + num;
        // Define the process
        Process p = Process(id, arrival);
        // Increment ids
        number++;
        // Find the CPU burst count
        int numCPU = ceil(drand48() * 32);
        for (int j = 0; j < numCPU; j++)
        {
            // CPU TASK
            int cputime = ceil(ed.next_exp());
            if (ncpu > 0)
            {
                cputime *= 4;
            }
            p.cpuBursts.push_back(cputime);
            // IO TASK (all but last iteration)
            if (j < numCPU - 1)
            {
                int iotime = ceil(ed.next_exp());
                if (ncpu <= 0)
                {
                    iotime *= 8;
                }
                p.ioBursts.push_back(iotime);
            }
        }
        // If this process was a CPU scheduled process, decrement that counter
        if (ncpu > 0)
        {
            ncpu--;
        }
        // Add process to list
        processes.push_back(p);
    }
    return processes;
}

void printProcesses(const vector<Process> &processes, int ncpu, int seed, double lambda, int bound)
{
    cout << "<<< PROJECT PART I" << endl;
    cout << "<<< -- process set (n=" << processes.size() << ") with " << ncpu << " CPU-bound process(es)" << endl;
    cout << "<<< -- seed=" << seed << "; lambda=" << lambda << "; bound=" << bound << endl;

    for (const auto &process : processes)
    {
        cout << (find(process.id.begin(), process.id.end(), '0') != process.id.end() ? "CPU-bound" : "I/O-bound")
             << " process " << process.id << ": arrival time " << process.arrivalTime << "ms; "
             << process.cpuBursts.size() << " CPU bursts:" << endl;

        for (size_t i = 0; i < process.cpuBursts.size(); ++i)
        {
            cout << "==> CPU burst " << process.cpuBursts[i] << "ms";
            if (i < process.ioBursts.size())
            {
                cout << " ==> I/O burst " << process.ioBursts[i] << "ms";
            }
            cout << endl;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        cerr << "Usage: " << argv[0] << " <n> <ncpu> <seed> <lambda> <bound>" << endl;
        return 1;
    }

    int n = stoi(argv[1]);
    int ncpu = stoi(argv[2]);
    int seed = stoi(argv[3]);
    double lambda = stod(argv[4]);
    int bound = stoi(argv[5]);

    ExponentialDistribution ed = ExponentialDistribution(seed, lambda, bound);

    auto processes = generateProcesses(n, ncpu, ed);
    printProcesses(processes, ncpu, seed, lambda, bound);

    return 0;
}