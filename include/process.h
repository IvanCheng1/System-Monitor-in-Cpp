#ifndef PROCESS_H
#define PROCESS_H

#include <string>


class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

 private:
  std::string kProcDirectory{"/proc/"};
  std::string kStatFilename{"/stat"};
  int pid_;
  float cpu_;
};

#endif
