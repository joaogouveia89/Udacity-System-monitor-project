#ifndef HELPERS_H
#define HELPERS_H

#include<string>
#include<vector>

namespace Helpers{
    //Strings

    // Got from http://www.cplusplus.com/articles/2wA0RXSz/
    const std::vector<std::string> split(const std::string& s, const char& separator);

    // Converter
    const std::string kbToMb(unsigned long &kb);
}
#endif