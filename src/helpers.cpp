#include "helpers.h"

const std::vector<std::string> Helpers::split(const std::string& s, const char& separator){
    std::string buff{""};
    std::vector<std::string> v;
    
    for(auto n:s)
    {
        if(n != separator) buff+=n; else
        if(n == separator && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);
    
    return v;
}

const std::string Helpers::kbToMb(unsigned long &kb){
    int mb = kb / 1000;
    return std::to_string(mb);
}
