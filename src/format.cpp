#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::FormatTime(int time) {
  string output = std::to_string(time);
  if (output.size() == 2) {
    return output;
  } else if (output.size() == 1) {
    return '0' + output;
  } else {
    return "00";
  }
};

string Format::ElapsedTime(long seconds) {
  int h, m, s;
  string output;

  h = seconds / 3600;
  seconds = seconds % 3600;
  m = seconds / 60;
  seconds = seconds % 60;
  s = seconds;

  output = FormatTime(h) + ":" + FormatTime(m) + ":" + FormatTime(s);

  return output;
}
