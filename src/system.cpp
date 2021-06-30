#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
    vector<int> currentPids = LinuxParser::Pids();
    processes_.clear();
    for(auto & pid : currentPids){
        processes_.push_back(Process(pid, UpTime()));
    }
    std::sort(processes_.begin(), processes_.end());
    std::reverse(processes_.begin(),processes_.end());
    return processes_; 
}

std::string System::Kernel() { return kernel.empty() ? kernel = LinuxParser::Kernel() : kernel; }

float System::MemoryUtilization() {return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return operatingSystem.empty() ? operatingSystem = LinuxParser::OperatingSystem() : operatingSystem; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long System::UpTime() {return LinuxParser::UpTime(); }