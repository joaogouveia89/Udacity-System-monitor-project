#include "linux_parser_mutable.h"

void LinuxParserMutable::fetchSystemData(){
    SystemMemoryUtilization();
    SystemUpTime();
    SystemStat();
    onFetchFinished();
}

// reference: https://en.cppreference.com/w/cpp/filesystem
vector<int> LinuxParserMutable::Pids() {
  vector<int> pids;
  for(auto& it : fs::directory_iterator(Paths::kProcDirectory)){
    auto itemStatus = fs::status(it);
    if(it.is_directory()){
      std::string filename = Helpers::split(it.path(), '/').back();
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  return pids;
}


void LinuxParserMutable::SystemMemoryUtilization() {
  std::ifstream stream(Paths::kProcDirectory + Paths::kMeminfoFilename);
  string line, key, value;
  bool hasMemoryTotal = false;
  bool hasMemoryFree = false;

  long long memTotal, memFree;
  if (stream.is_open()) {
    while (std::getline(stream, line) && !hasMemoryTotal && !hasMemoryFree) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if(key == SYSTEM_MEMORY_TOTAL){
          data_[SYSTEM_MEMORY_TOTAL] = value;
          hasMemoryTotal = true;
        }
        if(key == SYSTEM_MEMORY_FREE){
          data_[SYSTEM_MEMORY_FREE] = value;
          hasMemoryFree = true;
        }
      }
    }
  }
}

void LinuxParserMutable::SystemUpTime() {
  std::ifstream stream(Paths::kProcDirectory + Paths::kUptimeFilename);
  string line;
  long startTime = 0L;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::vector<std::string> brokeLine = Helpers::split(line, ' ');
    data_[SYSTEM_UP_TIME] = brokeLine[0];
  }
}

void LinuxParserMutable::SystemStat(){
    std::ifstream stream(Paths::kProcDirectory + Paths::kStatFilename);
    string line;
    int lineNumber;

    /* 
     * I could check if the map has the key regarding to each field, but I thought that the boolean check is faster,
     * even it is not more elegant than map checking
     */
    bool hasCpuUtilization = false;
    bool hasTotalProcesses = false;
    bool hasSystemRunningProcesses = false;

    while (std::getline(stream, line) && !hasCpuUtilization && !hasSystemRunningProcesses && !hasTotalProcesses) {
        vector<string> splittedLine = Helpers::split(line, ' ');
        if(splittedLine[0] == SYSTEM_CPU_UTILIZATION){
            data_[SYSTEM_CPU_UTILIZATION] = line;
            hasCpuUtilization = true;
        }

        if(splittedLine[0] == SYSTEM_TOTAL_PROCESSES){
            data_[SYSTEM_TOTAL_PROCESSES] = line;
            hasTotalProcesses = true;
        }

        if(splittedLine[0] == SYSTEM_RUNNING_PROCESSES){
            data_[SYSTEM_RUNNING_PROCESSES] = line;
            hasSystemRunningProcesses = true;
        }
    }
}