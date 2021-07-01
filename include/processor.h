#ifndef PROCESSOR_H
#define PROCESSOR_H

#include<string>
#include<vector>
#include "linux_parser_mutable.h"

using std::vector;
using std::string;

class Processor {
 public:
  void Utilization(string strData); 
  float Utilization(); 

 private:
    unsigned long prevIdle{ 0 };
    unsigned long prevNonIdle{ 0 };

    float utilization_ { 0.0 };

    //parsed data vector positions
    const int USER = 1;
    const int NICE = 2;
    const int SYSTEM = 3;
    const int IDLE = 4;
    const int IO_WAIT = 5;
    const int IRQ = 6;
    const int SOFT_IRQ = 7;
    const int STEAL = 8;
};

#endif