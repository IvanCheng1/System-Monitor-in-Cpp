#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// read data from the filesystem
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
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  stream.close();
  return kernel;
}

// get process ID
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float MemTotal, MemFree;
  string line, key;
  float value;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          MemTotal = value;
        } else if (key == "MemFree:") {
          MemFree = value;
        }
      }
    }
  }
  filestream.close();
  float output =  (MemTotal - MemFree) / MemTotal;

  return output;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long up_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time;
  }
  stream.close();
  return up_time;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::string line, cpu, value;
  std::vector<std::string> utilization;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> cpu;
      if (cpu == "cpu") {
        while (linestream >> value) {
          utilization.push_back(value);
        }
      }
    }
  }
  filestream.close();
  return utilization;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  filestream.close();
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, ram = "0";
  int value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmData:") { // changed from "VmSize"
          ram = to_string(value / 1024); // changed from 1000
          filestream.close();
          return ram;
        }
      }
    }
  }
  filestream.close();
  return ram;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {

  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {

  string line, user, x, current_uid;
  string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> current_uid) {
        if (uid == current_uid) {
          filestream.close();
          return user;
        }
      }
    }
  }
  filestream.close();
  return user;

}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {

  std::string line, starttime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    int count = 0;
    while (count < 22) {
      linestream >> starttime;
      count ++;
    }
  }
  filestream.close();
  long output = stof(starttime) / sysconf(_SC_CLK_TCK);

  return output;
}
