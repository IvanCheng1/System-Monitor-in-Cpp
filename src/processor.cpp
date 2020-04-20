#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
  
  std::string line, cpu, value;
  std::vector <std::string> utilization = LinuxParser::CpuUtilization();
  
  float user = stof(utilization[0]);
  float nice = stof(utilization[1]);
  float system = stof(utilization[2]);
  float idle = stof(utilization[3]);
  float iowait = stof(utilization[4]);
  float irq = stof(utilization[5]);
  float softirq = stof(utilization[6]);
  float steal = stof(utilization[7]);
//   float guest = stof(utilization[8]);
//   float guest_nice = stof(utilization[9]);

  
  float PrevIdle = previdle + previowait;
  float Idle = idle + iowait;
  
  float PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
  float NonIdle = user + nice + system + irq + softirq + steal;
  
  float PrevTotal = PrevIdle + PrevNonIdle;
  float Total = Idle + NonIdle;
  
  float totald = Total - PrevTotal;
  float idled = Idle - PrevIdle;
  
  float output = (totald - idled) / totald;
  
  previdle = idle;
  previowait = iowait;
  prevuser = user;
  prevnice = nice;
  prevsystem = system;
  previrq = irq;
  prevsoftirq = softirq;
  prevsteal = steal;
  
  return output; 
}