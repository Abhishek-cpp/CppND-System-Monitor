#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <utility>

class Processor {
 public:
  float Utilization();

 private:
  void GetIdleTime();

  std::pair<long, long> time_pair{0,0};
  long prev_idle_{0};
  long prev_total_{0};
};

#endif