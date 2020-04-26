#include "format.h"

#include <string>

using std::string;

std::string Format::zero_padding(std::string time_segment) {
  constexpr auto padding_size = 2;
  if (time_segment.length() < 2) {
    time_segment.insert(time_segment.begin(), padding_size - time_segment.size(),
                        '0');
  }
  return time_segment;
}

std::string Format::ElapsedTime(long seconds) {
  return zero_padding(std::to_string(seconds / 3600)) + ":" +
         zero_padding(std::to_string((seconds / 60) % 60)) + ":" +
         zero_padding(std::to_string(seconds % 60));
}