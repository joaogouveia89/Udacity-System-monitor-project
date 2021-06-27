#ifndef PROCESSOR_H
#define PROCESSOR_H

#include<string>
#include<vector>
#include "linux_parser.h"

using std::vector;
using std::string;

class Processor {
 public:
  float Utilization(); 

 private:
    long prevIdle{ 0 };
    long prevIoWait{ 0 };
    long prevUser{ 0 };
    long prevNice{ 0 };
    long prevSystem{ 0 };
    long prevIrq{ 0 };
    long prevSoftIrq{ 0 };
    long prevSteal{ 0 };

    // CPU
    enum CPUStates {
        kUser_ = 1,
        kNice_,
        kSystem_,
        kIdle_,
        kIOwait_,
        kIRQ_,
        kSoftIRQ_,
        kSteal_,
        kGuest_,
        kGuestNice_
    };

    vector<string> parseData();

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