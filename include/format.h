#ifndef FORMAT_H
#define FORMAT_H

#include <iostream>
#include <string>

namespace Format {
std::string zero_padding(std::string time_segment);
std::string ElapsedTime(long times);
};
#endif