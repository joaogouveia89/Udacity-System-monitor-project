#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include "linux_parser_mutable.h"
#include "identifiers.h"

using std::vector;
using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process : public LinuxParserMutable{
 public:
  Process(int pid, long systemUptime);

  int Pid() const;
  string User() const;
  string Command() const;
  float CpuUtilization() const;
  string Ram() const;
  long UpTime() const;
  bool operator<(Process const& a) const;

  void updateProcessInformation(long systemUptime);

  void onFetchFinished() override;

 private:
    int pid_;
    float cpuUtilization_{ 0.0 };
    long systemUpTime { 0 };

    vector<string> parseCpuUtilization();

    void computeProcessCpuUtilization(long systemUptime);
};

#endif