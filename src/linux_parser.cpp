#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  // std::cout << "LinuxParser::MemoryUtilization()" << std::endl;

  float total_memory{0.0}, free_memory{0.0};
  std::string key{""}, value{""}, line{""};
  if (std::ifstream filestream(kProcDirectory + kMeminfoFilename);
      filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          total_memory = std::stof(value);
        }
        if (key == "MemFree") {
          free_memory = std::stof(value);
        }
      }
    }
  }

  return (total_memory - free_memory) / total_memory;
}

long LinuxParser::UpTime() {
  // std::cout << "LinuxParser::UpTime()" << std::endl;
  long uptime{0};
  std::string time{""}, line{""};
  if (std::ifstream filestream(kProcDirectory + kUptimeFilename);
      filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> time;
    uptime = std::stol(time);
  }
  return uptime;
}

long LinuxParser::ActiveJiffies(int pid) {
  // std::cout << "LinuxParser::ActiveJiffies(int pid)" << std::endl;
  long index{0}, active_jiffies{0};
  std::string value{""}, line{""};
  if (std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                               kStatFilename);
      filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (index >= 13 and index <= 16) {
        active_jiffies += std::stol(value);
      }
      ++index;
    }
  }

  return active_jiffies;
}

vector<string> LinuxParser::CpuUtilization() {
  // std::cout << "LinuxParser::CpuUtilization()" << std::endl;
  string line{""}, key{""};
  vector<string> jiffies{};
  if (std::ifstream stream(kProcDirectory + kStatFilename); stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> key) {
      if (key != "cpu") {
        jiffies.push_back(key);
      }
    }
  }
  return jiffies;
}

int LinuxParser::CountProcess(std::string type_of_process) {
  // std::cout << "LinuxParser::CountProcess(std::string type_of_process) = "
  //           << type_of_process << std::endl;
  int num_of_processes{0};
  string key{""}, value{""}, line{""};
  if (std::ifstream stream(kProcDirectory + kStatFilename); stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == type_of_process) {
          num_of_processes = std::stoi(value);
        }
      }
    }
  }
  return num_of_processes;
}

int LinuxParser::TotalProcesses() { return CountProcess("processes"); }

int LinuxParser::RunningProcesses() { return CountProcess("procs_running"); }

string LinuxParser::Command(int pid) {
  // std::cout << "LinuxParser::Command(int pid)" << std::endl;
  string command{""}, line{""};
  if (std::ifstream stream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
      stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      std::replace(line.begin(), line.end(), ' ', '*');
      while (linestream >> command) {
        std::replace(command.begin(), command.end(), '*', ' ');
        break;
      }
    }
  }
  return command;
}

std::string LinuxParser::Ram(int pid) {
  // std::cout << "LinuxParser::Ram(int pid) " << std::endl;
  std::string key{""}, value{""}, line{""}, memory_usage{""};
  if (std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                               kStatusFilename);
      filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          memory_usage = std::to_string(std::stoi(value) / 1024);
        }
      }
    }
  }

  return memory_usage;
}

string LinuxParser::Uid(int pid) {
  // std::cout << "LinuxParser::Uid(int pid)" << std::endl;
  string key{""}, value{""}, uid{""}, line{""};
  if (std::ifstream stream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
      stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;
        }
      }
    }
  }
  return uid;
}

string LinuxParser::User(int pid) {
  // std::cout << "LinuxParser::User(int pid)" << std::endl;
  string user{""}, password{""}, uid{""}, line{""};
  if (std::ifstream stream(kPasswordPath); stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> password >> uid) {
        if (uid == LinuxParser::Uid(pid)) {
          return user;
        }
      }
    }
  }
  return string();
}

long LinuxParser::UpTime(int pid) {
  // std::cout << "LinuxParser::User(int pid)" << std::endl;
  long index{0}, time{0};
  std::string value{""}, line{""};
  if (std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                               kStatFilename);
      filestream.fail()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (index == 21) {
        time = std::stol(value) / sysconf(_SC_CLK_TCK);
        break;
      }
      ++index;
    }
  }

  return LinuxParser::UpTime() - time;
}
