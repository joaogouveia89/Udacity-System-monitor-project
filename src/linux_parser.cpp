#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "helpers.h"
#include <filesystem>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

namespace fs = std::filesystem;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// reference: https://en.cppreference.com/w/cpp/filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  for(auto& it : fs::directory_iterator(kProcDirectory)){
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


float LinuxParser::MemoryUtilization() {
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string line, key, value;
  //TODO: Add logic to interrupt the file reading to avoid passing through all the lines
  long long memTotal, memFree;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if(key == "MemTotal"){
          memTotal = strtoll(value.c_str(), nullptr, 10);
        }
        if(key == "MemFree"){
          memFree = strtoll(value.c_str(), nullptr, 10);
        }
      }
    }
  }
  return (memTotal - memFree) / (float)memTotal; 
}

long LinuxParser::UpTime() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  string line;
  long startTime = 0L;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::vector<std::string> brokeLine = Helpers::split(line, ' ');
    startTime =  stol(brokeLine[0]);
  }
  return startTime;
}

vector<string> LinuxParser::CpuUtilization() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  if (stream.is_open()) {
    std::getline(stream, line);
    return Helpers::split(line, ' ');
  }
  return {}; 
}

int LinuxParser::TotalProcesses() {
   std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  if (stream.is_open()) {
    string key;
    string value;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

int LinuxParser::RunningProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  if (stream.is_open()) {
    string key;
    string value;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

string LinuxParser::Command(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line;
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string();
}


string LinuxParser::Ram(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  if (stream.is_open()) {
    string key;
    string value;
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          return value;
        }
      }
    }
  }
  return string(); 
}

string LinuxParser::Uid(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;

  if (stream.is_open()) {
    string key;
    string value;
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          return value;
        }
      }
    }
  }
  return string();
}

string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  string line;
  if (stream.is_open()) {
    string usernameUid;
    string username;
    string x;
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> x >> usernameUid) {
        if(usernameUid == uid){
          return username;
        }
      }
    }
  }

  return string();
}

long int LinuxParser::UpTime(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  string line;
  long startTime = 0L;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::vector<std::string> brokeLine = Helpers::split(line, ' ');
    startTime =  stol(brokeLine[21]);
  }
  return startTime / sysconf(_SC_CLK_TCK);
}

vector<string> LinuxParser::parseCpuUtilization(int pid){
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  string line;
  if (stream.is_open()) {
    std::getline(stream, line);
    return Helpers::split(line, ' ');
  }
  return {}; 
}
 