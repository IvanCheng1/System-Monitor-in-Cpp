#include <unistd.h>
#include <cctype>
#include <sstream> 
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  CpuUtilization();
}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
  std::string line, value;
  std::vector<string> utilization;
  std::ifstream filestream(kProcDirectory + to_string(pid_) + kStatFilename);
  
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
      
    int count = 0;
    while (count < 22) {
      linestream >> value;
      utilization.push_back(value);
      count ++;
    }
  }
  
  float utime = stof(utilization[13]);
  float stime = stof(utilization[14]);
  float cutime = stof(utilization[15]);
  float cstime = stof(utilization[16]);
  float starttime = stof(utilization[21]);
    
  float Hertz = sysconf(_SC_CLK_TCK);
  float uptime = LinuxParser::UpTime(); 
  
  float total_time = utime + stime + cutime + cstime;
  float seconds = uptime - (starttime / Hertz);
  
  float cpu_usage = ((total_time / Hertz) / seconds);
  
  cpu_ = cpu_usage;
  
  return cpu_usage; 
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {  
  return a.cpu_ < this->cpu_;
}
