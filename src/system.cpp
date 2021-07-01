#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "linux_parser_unmutable.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
    vector<int> currentPids = Pids();
    processes_.clear();
    for(auto & pid : currentPids){
        processes_.push_back(Process(pid, UpTime()));
    }
    std::sort(processes_.begin(), processes_.end());
    std::reverse(processes_.begin(),processes_.end());
    return processes_; 
}

std::string System::Kernel() { return kernel.empty() ? kernel = LinuxParserUnmutable::Kernel() : kernel; }

float System::MemoryUtilization() {return memoryUtilization_; }

std::string System::OperatingSystem() { return operatingSystem.empty() ? operatingSystem = LinuxParserUnmutable::OperatingSystem() : operatingSystem; }

int System::RunningProcesses() { return runningProcesses_; }

int System::TotalProcesses() { return totalProcesses_; }

long System::UpTime() {return uptTime_; }

void System::onFetchFinished(){
    memoryUtilization_ = calculateMemoryUtilization();
    cpu_.Utilization(Data(SYSTEM_CPU_UTILIZATION));

    vector<string> runningProcessesUnformmatted = Helpers::split(Data(SYSTEM_RUNNING_PROCESSES), ' ');
    runningProcesses_ = stoi(runningProcessesUnformmatted[1]);

    vector<string> totalrocessesUnformmatted = Helpers::split(Data(SYSTEM_TOTAL_PROCESSES), ' ');
    totalProcesses_ = stoi(totalrocessesUnformmatted[1]);

    std::vector<std::string> uptTimeUnformmatted = Helpers::split(Data(SYSTEM_UP_TIME), ' ');
    uptTime_ = stol(uptTimeUnformmatted[0]);
}

float System::calculateMemoryUtilization(){
    long long memTotal =  strtoll(Data(SYSTEM_MEMORY_TOTAL).c_str() , nullptr, 10);
    long long memFree =  strtoll(Data(SYSTEM_MEMORY_FREE).c_str() , nullptr, 10);

    return (memTotal - memFree) / (float)memTotal;
}