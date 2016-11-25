#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <fstream>
#include <ibex.h>

class IntervalsWriter {
private:
  std::string accepted_boxes_file_path;
  std::string rejected_boxes_file_path;
  IntervalsWriter();
  void Write(ibex::IntervalVector box, std::ofstream&);
public:
  IntervalsWriter(std::string accepted_boxes_file_path, 
                  std::string rejected_boxes_file_path);
  void WriteAccepted(ibex::IntervalVector accepted_box);
  void WriteRejected(ibex::IntervalVector rejected_box);
};

#endif
