#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>

class Processor {
 public:
  float Utilization();

 private:
  const std::string kProcDirectory{"/proc/"};
  const std::string kStatFilename{"/stat"};
  float previdle = 0.0;
  float previowait = 0.0;
  float prevuser = 0.0;
  float prevnice = 0.0;
  float prevsystem = 0.0;
  float previrq = 0.0;
  float prevsoftirq = 0.0;
  float prevsteal = 0.0;
  };

#endif
