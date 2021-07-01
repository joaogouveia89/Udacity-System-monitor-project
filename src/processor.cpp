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

float Processor::Utilization() {
    return utilization_;
}

void Processor::Utilization(string strData){
    vector<string> data = Helpers::split(strData, ' ');

    unsigned long puser = std::strtoul(data[USER].c_str(), nullptr, 0);
    unsigned long pnice = std::strtoul(data[NICE].c_str(), nullptr, 0);
    unsigned long psystem = std::strtoul(data[SYSTEM].c_str(), nullptr, 0);
    unsigned long pidle = std::strtoul(data[IDLE].c_str(), nullptr, 0);
    unsigned long pioWait = std::strtoul(data[IO_WAIT].c_str(), nullptr, 0);
    unsigned long pirq = std::strtoul(data[IRQ].c_str(), nullptr, 0);
    unsigned long psoftIrq = std::strtoul(data[SOFT_IRQ].c_str(), nullptr, 0);
    unsigned long psteal = std::strtoul(data[STEAL].c_str(), nullptr, 0);

    unsigned long idle = pidle + pioWait;
    unsigned long nonIdle = puser + pnice + psystem + pirq + psoftIrq + psteal;

    unsigned long prevTotal = prevIdle + prevNonIdle;
    unsigned long total = idle + nonIdle;

    unsigned long totalD = total - prevTotal;
    unsigned long idleD = idle - prevIdle;

    prevIdle = idle;
    prevNonIdle = nonIdle;

    utilization_ = (totalD - idleD) /(float)totalD;
}
