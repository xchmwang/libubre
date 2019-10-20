#include "runtime/version.h"
#include <string>

std::string entry_point_exp(const std::string &msg) {
  neb::rt::get_version();
  return "ret:" + msg;
}
