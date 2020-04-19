#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // DONE: See src/format.cpp
};                                    // namespace Format

namespace Format {
std::string FormatTime(int time);  
}; 

#endif