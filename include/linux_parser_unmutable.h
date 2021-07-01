#ifndef LINUX_PARSER_UNMUTABLE_H
#define LINUX_PARSER_UNMUTABLE_H

#include <fstream>
#include <regex>
#include <string>
#include "paths.h"

namespace LinuxParserUnmutable {
    std::string OperatingSystem();
    std::string Kernel();
};

#endif