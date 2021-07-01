#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "linux_parser_mutable.h"

class System : public LinuxParserMutable{
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

 private:
  std::string operatingSystem { "" };
  std::string kernel { "" };
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};

  float memoryUtilization_{ 0.0 };
  int runningProcesses_ { 0 };
  int totalProcesses_{ 0 };
  long uptTime_{ 0 };

  float calculateMemoryUtilization();

  void onFetchFinished() override;
};

#endif