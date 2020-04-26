#include <algorithm>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

Processor& System::Cpu() { return cpu_; }

std::vector<Process>& System::Processes() {
  processes_.clear();
  for (int pid : LinuxParser::Pids()) {
    processes_.emplace_back(Process{pid});
  }
  std::sort(processes_.begin(), processes_.end());
  return processes_;
}

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

long int System::UpTime() { return LinuxParser::UpTime(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

std::string System::Kernel() { return LinuxParser::Kernel(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }
