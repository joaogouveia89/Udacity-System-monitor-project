#ifndef IDENTIFIERS_H
#define IDENTIFIERS_H
#include<string>

using std::string;

namespace Identifiers{
    const static string SYSTEM_MEMORY_TOTAL = "MemTotal";
    const static string SYSTEM_MEMORY_FREE = "MemFree";
    const static string SYSTEM_UP_TIME = "Uptime";
    const static string SYSTEM_CPU_UTILIZATION = "cpu";
    const static string SYSTEM_TOTAL_PROCESSES = "processes";
    const static string SYSTEM_RUNNING_PROCESSES = "procs_running";

    const static string PROCESS_UID = "Uid";
    const static string PROCESS_USERNAME = "Username";
    const static string PROCESS_RAM = "VmRSS"; //To project reviewer: The VmRSS returns me the exact physical memory being used as a part of Physical RAM, and VmSize as it was described on the project requirements, returns me Virtual memory size, so that is the reason I changed from VmSize to VmRSS
    const static string PROCESS_START_TIME = "StartTime";
    const static string PROCESS_U_TIME = "U_TIME";
    const static string PROCESS_S_TIME = "S_TIME";
    const static string PROCESS_CU_TIME = "CU_TIME";
    const static string PROCESS_CS_TIME = "CS_TIME";
    const static string PROCESS_COMMAND = "PROCESS_COMMAND";

}

#endif