#include "process.h"

#include <unistd.h>

#include <string>

#include "linux_parser.h"

Process::Process(int pid) : pid_(pid) {}

int Process::Pid() const { return pid_; }

std::string Process::User() const { return LinuxParser::User(pid_); }

std::string Process::Command() const { return LinuxParser::Command(pid_); }

float Process::CpuUtilization() const {
  float time_elapsed = LinuxParser::ActiveJiffies(pid_)/ sysconf(_SC_CLK_TCK);
  float time_in_seconds = LinuxParser::UpTime(pid_);
  return time_elapsed/time_in_seconds;
}

std::string Process::Ram() const { return LinuxParser::Ram(pid_); }

long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }

bool Process::operator<(const Process& a) const {
  return this->CpuUtilization() > a.CpuUtilization();
}