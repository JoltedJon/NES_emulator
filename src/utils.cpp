#include "utils.h"

#include <iomanip>
#include <ios>
#include <sstream>

template <typename T>
std::string to_hex(T i) {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(sizeof(T) * 2) << std::uppercase
     << std::hex << static_cast<int>(i);
  return ss.str();
}
