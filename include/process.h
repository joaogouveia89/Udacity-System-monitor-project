#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

using std::vector;
using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
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

 private:
    int pid_;
    string user_;
    string command_;
    float cpuUtilization_{ 0.0 };
    string ram_{ "" };
    long int uptime_{ 0L };

    vector<string> parseCpuUtilization();

    float computeProcessCpuUtilization(long systemUptime);

    //parsed array positions

    const static int U_TIME = 13;
    const static int S_TIME = 14;
    const static int CU_TIME = 15;
    const static int CS_TIME = 16;
    const static int START_TIME = 21;
};

#endif