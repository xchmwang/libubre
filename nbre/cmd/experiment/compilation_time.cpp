#include "common/common.h"
#include "common/configuration.h"
#include "fs/util.h"
#include "util/chrono.h"
#include "util/command.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

std::vector<std::string> split_by_comma(const std::string &str, char comma) {
  std::vector<std::string> v;
  std::stringstream ss(str);
  std::string token;

  while (getline(ss, token, comma)) {
    v.push_back(token);
  }
  return v;
}

std::string dep_files_path(const std::string &dir,
                           const std::string &dep_files_name) {
  auto v = split_by_comma(dep_files_name, ',');
  std::string ret;
  for (auto &dep : v) {
    ret += neb::fs::join_path(dir, dep);
    ret += ' ';
  }
  return ret;
}

int main(int argc, char *argv[]) {

  po::options_description desc("Binary compilation time");
  desc.add_options()("help", "show help message")(
      "input", po::value<std::string>(), "input file to compile")(
      "deps", po::value<std::string>(), "dependencies");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }

  if (!vm.count("input")) {
    std::cout << "You must specify \"input\"!" << std::endl;
    return 1;
  }

  std::string nbre_path = ::neb::configuration::instance().nbre_root_dir();
  std::string filepath = neb::fs::join_path(nbre_path, "/cmd/experiment/");
  std::string includepath = neb::fs::join_path(nbre_path, "/lib/include/");
  std::string libpath = neb::fs::join_path(nbre_path, "/lib/lib/");

  std::string filename = vm["input"].as<std::string>();
  std::string deps;
  if (vm.count("deps")) {
    deps = vm["deps"].as<std::string>();
    std::string deppath = neb::fs::join_path(nbre_path, "/ir/experiment/");
    deps = dep_files_path(deppath, deps);
  }
  std::string command_string("clang++ -std=c++14 -O2 -lglog -lnbre_rt "
                             "-lboost_thread -lboost_system -lprotobuf -I" +
                             includepath + " -L" + libpath + " " +
                             neb::fs::join_path(filepath, filename) + " " +
                             deps + " -o " + filename + ".out");
  LOG(INFO) << command_string;
  auto result = neb::util::command_executor::execute_command(command_string);
  LOG(INFO) << "compilation done";
  if (result != 0) {
    LOG(ERROR) << "error: executed by boost::process::system.";
    LOG(ERROR) << "result code = " << result;
    return -1;
  }
  return 0;
}
