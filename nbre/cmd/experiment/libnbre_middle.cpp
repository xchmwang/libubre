
#include "runtime/util.h"
#include "util/json_parser.h"

std::string meta_info_to_json(
    const std::vector<std::pair<std::string, std::string>> &meta_info) {
  boost::property_tree::ptree pt;
  for (auto &ele : meta_info) {
    pt.put(ele.first, ele.second);
  }
  std::string ret;
  neb::util::json_parser::write_json(ret, pt);
  return ret;
}

std::vector<std::pair<std::string, std::string>>
json_to_meta_info(const std::string &json) {
  boost::property_tree::ptree pt;
  neb::util::json_parser::read_json(json, pt);

  std::vector<std::pair<std::string, std::string>> meta_info;
  std::string start_height = pt.get<std::string>("start_height");
  std::string end_height = pt.get<std::string>("end_height");
  std::string version = pt.get<std::string>("version");

  meta_info.push_back(std::make_pair("start_height", start_height));
  meta_info.push_back(std::make_pair("end_height", end_height));
  meta_info.push_back(std::make_pair("version", version));
  return meta_info;
}

int main() {
  auto p = std::make_pair("key", "val");
  std::vector<std::pair<std::string, std::string>> v;
  v.push_back(p);
  std::string ss = meta_info_to_json(v);
  auto ret = json_to_meta_info(ss);
  return 0;
}
