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