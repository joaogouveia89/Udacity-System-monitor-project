#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, long systemUptime) : pid_(pid){
    user_ = LinuxParser::User(pid);
    command_ = LinuxParser::Command(pid);
    uptime_ = LinuxParser::UpTime(pid);
    cpuUtilization_ = computeProcessCpuUtilization(systemUptime);
}

void Process::updateProcessInformation(long systemUptime){
    uptime_ = LinuxParser::UpTime(pid_);
    cpuUtilization_ = computeProcessCpuUtilization(systemUptime);
}

float Process::computeProcessCpuUtilization(long systemUptime){
    vector<string> data = parseCpuUtilization();

    long Hertz = sysconf(_SC_CLK_TCK);

    unsigned long uTime = std::strtoul(data[U_TIME].c_str(), nullptr, 0);
    unsigned long sTime = std::strtoul(data[S_TIME].c_str(), nullptr, 0);
    unsigned long cuTime = std::strtoul(data[CU_TIME].c_str(), nullptr, 0);
    unsigned long csTime = std::strtoul(data[CS_TIME].c_str(), nullptr, 0);
    unsigned long startTime = std::strtoul(data[START_TIME].c_str(), nullptr, 0);

    unsigned long totalTime = uTime + sTime + cuTime + csTime;
    unsigned long seconds = systemUptime - (startTime / Hertz);

    return ((totalTime /(float) Hertz) /(float) seconds);
}

vector<string> Process::parseCpuUtilization(){ return LinuxParser::parseCpuUtilization(pid_); }

int Process::Pid() const{ return pid_; }

float Process::CpuUtilization() const{ return cpuUtilization_; }

string Process::Command() const{ return command_; }

string Process::Ram() const{ return ram_; }

string Process::User() const{ return user_; }

long int Process::UpTime() const{ return uptime_; }

bool Process::operator<(Process const& a) const {
     return this->CpuUtilization() < a.CpuUtilization(); 
}