#ifndef LINUX_PARSER_MUTABLE_H
#define LINUX_PARSER_MUTABLE_H

#include <map>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <regex>
#include <fstream>

#include "helpers.h"
#include "paths.h"

using std::vector;
using std::string;
namespace fs = std::filesystem;

class LinuxParserMutable
{
private:
    // System
    void SystemMemoryUtilization();
    void SystemUpTime();
    void SystemStat();

    // Processes
    std::string Command(int pid);
    std::string Ram(int pid);
    std::string Uid(int pid);
    std::string User(int pid);
    long int UpTime(int pid);
    std::vector<std::string> CpuUtilization(int pid);

    std::map<string, string> data_;

public:
    std::vector<int> Pids();
    void fetchSystemData();
    virtual void onFetchFinished() = 0;

    string Data(string key) const;

    const string SYSTEM_MEMORY_TOTAL = "MemTotal";
    const string SYSTEM_MEMORY_FREE = "MemFree";
    const string SYSTEM_UP_TIME = "Uptime";
    const string SYSTEM_CPU_UTILIZATION = "cpu";
    const string SYSTEM_TOTAL_PROCESSES = "processes";
    const string SYSTEM_RUNNING_PROCESSES = "procs_running";
};

#endif