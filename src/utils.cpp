#include <string>
#include <ibex.h>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <cstdlib>
#include <stdexcept>

#include <utils.hpp>

static bool check_dir_name(std::string file_path) {
  size_t filename_pos = file_path.find_last_of("/") + 1;
  std::string dir_path = file_path.substr(0, filename_pos);
  if (NULL == opendir(dir_path.c_str())) {
    return false;
  }
  return true;
}

static bool create_file(std::string file_path) {
  size_t filename_pos = file_path.find_last_of("/") + 1;
  std::string file_name = file_path.substr(filename_pos, 
                                              file_path.size() - filename_pos);
  std::ofstream file;
  file.open(file_path, std::ios::out | std::ios::trunc);
  if (!file.is_open()) {
    std::string msg = "File " + file_path + " cannot be opened";
    throw new std::runtime_error(msg);
  }
  file.close();
  return true;
}

IntervalsWriter::IntervalsWriter(std::string accepted_boxes_file_path, 
                                  std::string rejected_boxes_file_path) {
  if(!check_dir_name(accepted_boxes_file_path) 
      || !check_dir_name(rejected_boxes_file_path)) {
    std::cout << "ERROR Output directory does not exist" << std::endl;
    exit(1);
  }
  if (!create_file(accepted_boxes_file_path) 
      || !create_file(rejected_boxes_file_path)) {
    std::cout << "ERROR Output file cannot be created << std::endl";
    exit(1);
  }
  this->accepted_boxes_file_path = accepted_boxes_file_path;
  this->rejected_boxes_file_path = rejected_boxes_file_path;
}

void IntervalsWriter::WriteAccepted(ibex::IntervalVector accepted_box) {
  std::ofstream file;
  file.open(accepted_boxes_file_path, std::ios::out | std::ios::app);
  Write(accepted_box, file);
  file.close();
}

void IntervalsWriter::WriteRejected(ibex::IntervalVector rejected_box) {
  std::ofstream file;
  file.open(rejected_boxes_file_path, std::ios::out | std::ios::app);
  Write(rejected_box, file);
  file.close();
}

void IntervalsWriter::Write(ibex::IntervalVector box, std::ofstream &file) {
  file << box << std::endl;
}
