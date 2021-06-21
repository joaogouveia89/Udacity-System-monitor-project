#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() const{ return pid_; }

float Process::CpuUtilization() const{ return cpuUtilization_; }

void Process::CpuUtilization(float cpuUtilization){ cpuUtilization_ = cpuUtilization; }

string Process::Command() const{ return command_; }

string Process::Ram() const{ return ram_; }

void Process::Ram(std::string ram) { ram_ = ram; }

string Process::User() const{ return user_; }

long int Process::UpTime() const{ return uptime_; }

void Process::UpTime(long int uptime){ uptime_ = uptime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }