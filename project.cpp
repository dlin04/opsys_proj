#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <random>
#include <iomanip>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

class Process
{
public:
    string id;
    int arrivalTime;
    bool CPUBound;
    vector<int> cpuBursts;
    vector<int> ioBursts;

    Process(string id, int arrivalTime, bool CPUBound) : id(id), arrivalTime(arrivalTime), CPUBound(CPUBound) {}
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
        Process p = Process(id, arrival,ncpu > 0);
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

void printProcesses(const vector<Process> &processes, int ncpu, int seed, double lambda, int bound, map<string, int> &measurements)
{
    cout << "<<< PROJECT PART I" << endl;
    cout << "<<< -- process set (n=" << processes.size() << ") with " << ncpu << " CPU-bound process" <<  (ncpu > 1 || ncpu == 0 ? "es" : "") << endl;
    cout << "<<< -- seed=" << seed << "; lambda=" << fixed << setprecision(6) << lambda << "; bound=" << bound << endl;

    for (Process process : processes)
    {
        bool is_CPU_bound = process.CPUBound;
        cout << (is_CPU_bound ? "CPU-bound" : "I/O-bound")
             << " process " << process.id << ": arrival time " << process.arrivalTime << "ms; "
             << process.cpuBursts.size() << " " << (process.cpuBursts.size() == 1 ? "CPU burst:" : "CPU bursts:") << endl;

        for (long unsigned int i = 0; i < process.cpuBursts.size(); i++)
        {
            cout << "==> CPU burst " << process.cpuBursts[i] << "ms";
            
            measurements[is_CPU_bound ? "cpu-bound-cpu-time" : "io-bound-cpu-time"] += process.cpuBursts[i];
            measurements[is_CPU_bound ? "num-cpu-bound-cpu-bursts" : "num-io-bound-cpu-bursts"] += 1;
            
            if (i < process.ioBursts.size())
            {
                cout << " ==> I/O burst " << process.ioBursts[i] << "ms";
                measurements[is_CPU_bound ? "cpu-bound-io-time" : "io-bound-io-time"] += process.ioBursts[i];
                measurements[is_CPU_bound ? "num-cpu-bound-io-bursts" : "num-io-bound-io-bursts"] += 1;
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

    map<string, int> measurements;
    measurements["cpu-bound-cpu-time"] = 0;
    measurements["cpu-bound-io-time"] = 0;
    measurements["io-bound-cpu-time"] = 0;
    measurements["io-bound-io-time"] = 0;
    measurements["num-cpu-bound-cpu-bursts"] = 0;
    measurements["num-io-bound-cpu-bursts"] = 0;
    measurements["num-cpu-bound-io-bursts"] = 0;
    measurements["num-io-bound-io-bursts"] = 0;

    auto processes = generateProcesses(n, ncpu, ed);
    printProcesses(processes, ncpu, seed, lambda, bound, measurements);


    // create simout file to write averages to
    ofstream simout("simout.txt");
    if(simout.is_open()){
        double cpu_bound_cpu_avg = (measurements["num-cpu-bound-cpu-bursts"] > 0) ? 
            measurements["cpu-bound-cpu-time"] / double(measurements["num-cpu-bound-cpu-bursts"]) : 0;
        double io_bound_cpu_avg = (measurements["num-io-bound-cpu-bursts"] > 0) ?
            measurements["io-bound-cpu-time"] / double(measurements["num-io-bound-cpu-bursts"]) : 0;
        double overall_cpu_avg = ((measurements["num-cpu-bound-cpu-bursts"] + 
            measurements["num-io-bound-cpu-bursts"]) > 0) ?
            (measurements["cpu-bound-cpu-time"] + measurements["io-bound-cpu-time"]) / 
            double(measurements["num-cpu-bound-cpu-bursts"] + measurements["num-io-bound-cpu-bursts"]) : 0;
        double cpu_bound_io_avg = (measurements["num-cpu-bound-io-bursts"] > 0) ?
            measurements["cpu-bound-io-time"] / double(measurements["num-cpu-bound-io-bursts"]) : 0;
        double io_bound_io_avg = (measurements["num-io-bound-io-bursts"] > 0) ?
            measurements["io-bound-io-time"] / double(measurements["num-io-bound-io-bursts"]) : 0;
        double overall_io_avg = ((measurements["num-cpu-bound-io-bursts"] + 
            measurements["num-io-bound-io-bursts"]) > 0) ?
            (measurements["io-bound-io-time"] + measurements["cpu-bound-io-time"]) /
            double(measurements["num-cpu-bound-io-bursts"] + measurements["num-io-bound-io-bursts"]) : 0;
            
        // ROUND UP TO 3 DECIMALS
        cpu_bound_cpu_avg = ceil(cpu_bound_cpu_avg*1000)/1000;
        io_bound_cpu_avg = ceil(io_bound_cpu_avg*1000)/1000;
        overall_cpu_avg = ceil(overall_cpu_avg*1000)/1000;
        cpu_bound_io_avg = ceil(cpu_bound_io_avg*1000)/1000;
        io_bound_io_avg = ceil(io_bound_io_avg*1000)/1000;
        overall_io_avg = ceil(overall_io_avg*1000)/1000;

        simout << "-- number of processes: " << n << endl;
        simout << "-- number of CPU-bound processes: " << ncpu << endl;
        simout << "-- number of I/O-bound processes: " << n - ncpu << endl;
        
        simout << fixed << setprecision(3);
        simout << "-- CPU-bound average CPU burst time: " << cpu_bound_cpu_avg << " ms" << endl;
        simout << "-- I/O-bound average CPU burst time: " << io_bound_cpu_avg << " ms" << endl;
        simout << "-- overall average CPU burst time: " << overall_cpu_avg << " ms" << endl;
        simout << "-- CPU-bound average I/O burst time: " << cpu_bound_io_avg << " ms" << endl;
        simout << "-- I/O-bound average I/O burst time: " << io_bound_io_avg << " ms" << endl;
        simout << "-- overall average I/O burst time: " << overall_io_avg << " ms" << endl;
    } else {
        cerr << "ERROR: unable to open simout.txt" << endl;
    }

    return 0;
}