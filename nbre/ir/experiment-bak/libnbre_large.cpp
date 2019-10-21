
#include "runtime/nr/impl/nr_impl.h"
#include <string>

std::string entry_point_exp(const std::string &msg) {
  neb::rt::nr::entry_point_nr_impl(1, 2, 3, 10, 20, 30, 40, 1.0, 2.0, 3.0);
  return "ret:" + msg;
}
