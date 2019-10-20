
#include <algorithm>
#include <string>
#include <vector>

std::string entry_point_exp(const std::string &msg) {
  std::vector<int> v({3, 2, 1});
  sort(v.begin(), v.end());
  unsigned long long ret = v[0];
  return "msg:" + msg + ",ret:" + std::to_string(ret);
}
