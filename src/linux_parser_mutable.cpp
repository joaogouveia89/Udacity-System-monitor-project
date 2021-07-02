#include "linux_parser_mutable.h"

LinuxParserMutable::LinuxParserMutable(){}

string LinuxParserMutable::Data(string key) const{
  if(data_.find(key) == data_.end()) return ""; // workaround to avoid out of range crashes on map object. Should be removed on the future when the crash reason is found
  return data_.at(key);
}

void LinuxParserMutable::ClearData(){
  data_.clear();
}

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

  if (stream.is_open()) {
    while (std::getline(stream, line) || (!hasMemoryTotal && !hasMemoryFree)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if(key == Identifiers::SYSTEM_MEMORY_TOTAL){
          data_[Identifiers::SYSTEM_MEMORY_TOTAL] = value;
          hasMemoryTotal = true;
        }
        if(key == Identifiers::SYSTEM_MEMORY_FREE){
          data_[Identifiers::SYSTEM_MEMORY_FREE] = value;
          hasMemoryFree = true;
        }
      }
    }
  }
}

void LinuxParserMutable::SystemUpTime() {
  std::ifstream stream(Paths::kProcDirectory + Paths::kUptimeFilename);
  string line;

  if (stream.is_open()) {
    std::getline(stream, line);
    std::vector<std::string> brokeLine = Helpers::split(line, ' ');
    data_[Identifiers::SYSTEM_UP_TIME] = brokeLine[0];
  }
}

void LinuxParserMutable::SystemStat(){
    std::ifstream stream(Paths::kProcDirectory + Paths::kStatFilename);
    string line;

    /* 
     * I could check if the map has the key regarding to each field, but I thought that the boolean check is faster,
     * even it is not more elegant than map checking
     */
    bool hasCpuUtilization = false;
    bool hasTotalProcesses = false;
    bool hasSystemRunningProcesses = false;

    while (std::getline(stream, line) || (!hasCpuUtilization && !hasSystemRunningProcesses && !hasTotalProcesses)) {
        vector<string> splittedLine = Helpers::split(line, ' ');
        if(splittedLine[0] == Identifiers::SYSTEM_CPU_UTILIZATION){
            data_[Identifiers::SYSTEM_CPU_UTILIZATION] = line;
            hasCpuUtilization = true;
        }

        if(splittedLine[0] == Identifiers::SYSTEM_TOTAL_PROCESSES){
            data_[Identifiers::SYSTEM_TOTAL_PROCESSES] = line;
            hasTotalProcesses = true;
        }

        if(splittedLine[0] == Identifiers::SYSTEM_RUNNING_PROCESSES){
            data_[Identifiers::SYSTEM_RUNNING_PROCESSES] = line;
            hasSystemRunningProcesses = true;
        }
    }
}

// Process

void LinuxParserMutable::fetchProcessData(int pid){
    ProcessStatus(pid);
    ProcessStat(pid);
    ProcessUser();
    ProcessCommand(pid);
    onFetchFinished();
}

void LinuxParserMutable::ProcessStatus(int pid) {
  std::ifstream stream(Paths::kProcDirectory + to_string(pid) + Paths::kStatusFilename);
  string line;

  bool hasProcessUid = false;
  bool hasProcessRam = false;

  if (stream.is_open()) {
    string key;
    string value;
    while (std::getline(stream, line) || (!hasProcessUid && !hasProcessRam)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == Identifiers::PROCESS_UID) {
          data_[Identifiers::PROCESS_UID] = value;
          hasProcessUid = true;
        }

        if(key == Identifiers::PROCESS_RAM){
            data_[Identifiers::PROCESS_RAM] = value;
            hasProcessRam = true;
        }
      }
    }
  }
}

void LinuxParserMutable::ProcessStat(int pid){
    std::ifstream stream(Paths::kProcDirectory + to_string(pid) + Paths::kStatFilename);
    string line;
    if (stream.is_open()) {
        std::getline(stream, line);
        vector<string> stats = Helpers::split(line, ' ');

        data_[Identifiers::PROCESS_START_TIME] = stats[21];
        data_[Identifiers::PROCESS_U_TIME] = stats[13];
        data_[Identifiers::PROCESS_S_TIME] = stats[14];
        data_[Identifiers::PROCESS_CU_TIME] = stats[15];
        data_[Identifiers::PROCESS_CS_TIME] = stats[16];
    }
}


void LinuxParserMutable::ProcessUser() {
  std::ifstream stream(Paths::kPasswordPath);
  string line;
  if (stream.is_open()) {
    string usernameUid;
    string username;
    string x;
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> x >> usernameUid) {
        if(usernameUid == data_[Identifiers::PROCESS_UID]){
          data_[Identifiers::PROCESS_USERNAME] = username;
        }
      }
    }
  }

}


void LinuxParserMutable::ProcessCommand(int pid) {
  std::ifstream stream(Paths::kProcDirectory + to_string(pid) + Paths::kCmdlineFilename);
  string line;
  if (stream.is_open()) {
    std::getline(stream, line);
    data_[Identifiers::PROCESS_COMMAND] = line;
  }
}
