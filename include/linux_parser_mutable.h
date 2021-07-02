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
#include "identifiers.h"

using std::vector;
using std::string;
using std::to_string;
namespace fs = std::filesystem;

class LinuxParserMutable
{
private:
    // System
    void SystemMemoryUtilization();
    void SystemUpTime();
    void SystemStat();

    // Processes
    void ProcessStatus(int pid);
    void ProcessStat(int pid);
    void ProcessUser();
    void ProcessCommand(int pid);

    std::map<string, string> data_;

public:
    LinuxParserMutable();
    std::vector<int> Pids();
    void fetchSystemData();
    void fetchProcessData(int pid);
    virtual void onFetchFinished() = 0;

    string Data(string key) const;
};

#endif