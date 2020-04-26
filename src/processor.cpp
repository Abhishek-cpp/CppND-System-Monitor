#include "processor.h"

#include <numeric>

#include "linux_parser.h"

void Processor::GetIdleTime() {
  std::vector<long> cpu_utilization_list{};

  for (const auto& util : LinuxParser::CpuUtilization()) {
    cpu_utilization_list.emplace_back(std::stol(util));
  }
  auto idle_time = cpu_utilization_list[LinuxParser::kIdle_] +
                   cpu_utilization_list[LinuxParser::kIOwait_];
  auto total_time = std::accumulate(cpu_utilization_list.begin(),
                                    cpu_utilization_list.end(), 0);
  time_pair = std::make_pair(idle_time, total_time);
}

float Processor::Utilization() {
  GetIdleTime();

  float idle_delta = std::get<0>(time_pair) - prev_idle_;
  float total_delta = std::get<1>(time_pair) - prev_total_;

  prev_idle_ = std::get<0>(time_pair);
  prev_total_ = std::get<1>(time_pair);

  return 1.0 - idle_delta / total_delta;
}