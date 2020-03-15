#include "runtime/util.h"
#include <string>

std::string entry_point_exp(const std::string &msg) {
  auto p = std::make_pair("key", "val");
  std::vector<std::pair<std::string, std::string>> v;
  v.push_back(p);
  std::string ss = neb::rt::meta_info_to_json(v);
  auto ret = neb::rt::json_to_meta_info(ss);
  return "ret:" + msg;
}
