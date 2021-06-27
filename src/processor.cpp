#include "processor.h"

/*

     user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0


PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Percentage = (totald - idled)/totald

*/

// Parse Data from linux system
vector<string> Processor::parseData(){ return LinuxParser::CpuUtilization(); }

float Processor::Utilization() {
    vector<string> data = parseData();

    long user = std::stol(data[USER]);
    long nice = std::stol(data[NICE]);
    long system = std::stol(data[SYSTEM]);
    long idle = std::stol(data[IDLE]);
    long ioWait = std::stol(data[IO_WAIT]);
    long irq = std::stol(data[IRQ]);
    long softIrq = std::stol(data[SOFT_IRQ]);
    long steal = std::stol(data[STEAL]);

    prevIdle = prevIdle + prevIoWait;
    idle = idle + ioWait;

    long prevNonIdle = prevUser + prevNice + prevSystem + prevIrq + prevSoftIrq + prevSteal;
    long nonIdle = user + nice + system + irq + softIrq + steal;

    long prevTotal = prevIdle + prevNonIdle;
    long total = idle + nonIdle;

    long totalD = total - prevTotal;
    long idleD = idle - prevIdle;

    prevIdle = idle;
    prevIoWait = ioWait;
    prevUser = user;
    prevNice = nice;
    prevSystem = system;
    prevIrq = irq;
    prevSoftIrq = prevSoftIrq;
    prevSteal = steal;

    return (totalD - idleD) /(float)totalD; 
}