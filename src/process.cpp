#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "helpers.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, long systemUptime) : pid_(pid){
    this->systemUpTime = systemUptime;
    fetchProcessData(pid);
}

void Process::computeProcessCpuUtilization(long systemUptime){

    long Hertz = sysconf(_SC_CLK_TCK);

    unsigned long uTime = std::strtoul(Data(Identifiers::PROCESS_U_TIME).c_str(), nullptr, 0);
    unsigned long sTime = std::strtoul(Data(Identifiers::PROCESS_S_TIME).c_str(), nullptr, 0);
    unsigned long cuTime = std::strtoul(Data(Identifiers::PROCESS_CU_TIME).c_str(), nullptr, 0);
    unsigned long csTime = std::strtoul(Data(Identifiers::PROCESS_CS_TIME).c_str(), nullptr, 0);
    unsigned long startTime = std::strtoul(Data(Identifiers::PROCESS_START_TIME).c_str(), nullptr, 0);

    unsigned long totalTime = uTime + sTime + cuTime + csTime;
    unsigned long seconds = systemUptime - (startTime / Hertz);

    cpuUtilization_= (totalTime /(float) Hertz) /(float) seconds;
}

int Process::Pid() const{ return pid_; }

float Process::CpuUtilization() const{ return cpuUtilization_; }

string Process::Command() const{ return Data(Identifiers::PROCESS_COMMAND); }

string Process::Ram() const{
    unsigned long ramKb = std::strtoul(Data(Identifiers::PROCESS_RAM).c_str(), nullptr, 0);
    return Helpers::kbToMb(ramKb);
}

string Process::User() const{ return Data(Identifiers::PROCESS_USERNAME); }

long int Process::UpTime() const{
    long startTime = stol(Data(Identifiers::PROCESS_START_TIME));
    return startTime / sysconf(_SC_CLK_TCK);
}

bool Process::operator<(Process const& a) const {
     return this->CpuUtilization() < a.CpuUtilization(); 
}

void Process::onFetchFinished(){
    computeProcessCpuUtilization(systemUpTime);
}