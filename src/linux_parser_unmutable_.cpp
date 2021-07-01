#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "helpers.h"

#include "linux_parser_unmutable.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParserUnmutable::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(Paths::kOSPath);
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

string LinuxParserUnmutable::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(Paths::kProcDirectory + Paths::kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
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
 