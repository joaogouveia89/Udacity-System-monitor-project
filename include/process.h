#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);

  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  void CpuUtilization(float cpuu);
  std::string Ram() const;
  void Ram(std::string ram);
  long UpTime() const;
  void UpTime(long uptime);
  bool operator<(Process const& a) const;

 private:
    const int pid_;
    std::string user_;
    std::string command_;
    float cpuUtilization_{ 0.0 };
    std::string ram_{ "" };
    long int uptime_{ 0L };
};

#endif