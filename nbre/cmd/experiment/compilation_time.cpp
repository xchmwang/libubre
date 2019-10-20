#include "common/common.h"
#include "common/configuration.h"
#include "fs/util.h"
#include "util/chrono.h"
#include "util/command.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {

  po::options_description desc("Binary compilation time");
  desc.add_options()("help", "show help message")(
      "input", po::value<std::string>(), "input file to compile");

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
  std::string filepath = neb::fs::join_path(nbre_path, "/cmd/compilation/");
  std::string includepath = neb::fs::join_path(nbre_path, "/lib/include/");
  std::string libpath = neb::fs::join_path(nbre_path, "/lib/lib/");

  std::string filename = vm["input"].as<std::string>();
  std::string command_string(
      "clang++ -std=c++14 -O2 -I" + includepath + " -L" + libpath + " " +
      neb::fs::join_path(filepath, filename) + " -o " + filename + ".out");
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
