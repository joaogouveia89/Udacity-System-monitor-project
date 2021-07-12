#include <unistd.h>
#include <cstddef>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "linux_parser_unmutable.h"
#include "system.h"
#include "identifiers.h"

using std::size_t;
using std::string;
using std::vector;

System::System(){
    fetchSystemData();
}

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
    vector<int> currentPids = Pids();
    processes_.clear();
    for(auto & pid : currentPids){
        processes_.push_back(Process(pid, UpTime(), kernelVersion));
    }
    std::sort(processes_.begin(), processes_.end());
    std::reverse(processes_.begin(),processes_.end());
    return processes_; 
}

void System::getKernelVersion(){
    vector<string> spplited = Helpers::split(kernel, '.');
    kernelVersion = stoi(spplited[0]) + stoi(spplited[1]) / static_cast<float>(10);
}

std::string System::Kernel() { 
    if(kernel.empty()){
        kernel = LinuxParserUnmutable::Kernel();
        getKernelVersion();
    }
    return kernel; 
}

float System::MemoryUtilization() {return memoryUtilization_; }

std::string System::OperatingSystem() { return operatingSystem.empty() ? operatingSystem = LinuxParserUnmutable::OperatingSystem() : operatingSystem; }

int System::RunningProcesses() { return runningProcesses_; }

int System::TotalProcesses() { return totalProcesses_; }

long System::UpTime() {return uptTime_; }

void System::onFetchFinished(){
    memoryUtilization_ = calculateMemoryUtilization();
    cpu_.Utilization(Data(Identifiers::SYSTEM_CPU_UTILIZATION));

    vector<string> runningProcessesUnformmatted = Helpers::split(Data(Identifiers::SYSTEM_RUNNING_PROCESSES), ' ');
    runningProcesses_ = stoi(runningProcessesUnformmatted[1]);

    vector<string> totalrocessesUnformmatted = Helpers::split(Data(Identifiers::SYSTEM_TOTAL_PROCESSES), ' ');
    totalProcesses_ = stoi(totalrocessesUnformmatted[1]);

    std::vector<std::string> uptTimeUnformmatted = Helpers::split(Data(Identifiers::SYSTEM_UP_TIME), ' ');
    uptTime_ = stol(uptTimeUnformmatted[0]);
}

float System::calculateMemoryUtilization(){
    long long memTotal =  strtoll(Data(Identifiers::SYSTEM_MEMORY_TOTAL).c_str() , nullptr, 10);
    long long memFree =  strtoll(Data(Identifiers::SYSTEM_MEMORY_FREE).c_str() , nullptr, 10);

    return (memTotal - memFree) / (float)memTotal;
}