
#include <string>

unsigned long long fab(unsigned long long n) {
  if (n == 0 || n == 1) {
    return 1;
  }
  return fab(n - 1) + fab(n - 2);
}

std::string entry_point_exp(const std::string &msg) {
  unsigned long long ret = fab(43);
  return "msg:" + msg + ",ret:" + std::to_string(ret);
}
