#include <string>
#include <sstream>
#include <iostream>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    int minutes = seconds / 60;
    seconds = seconds % 60;
    int hours = minutes / 60;
    minutes = minutes % 60;
    std::stringstream stream;
    if(hours < 10){
        stream << "0";
    }
    stream << hours << ":";

    if(minutes < 10){
        stream << "0";
    }
    stream << minutes << ":";

    if(seconds < 10){
        stream << "0";
    }
    stream << seconds;

    return stream.str();
}