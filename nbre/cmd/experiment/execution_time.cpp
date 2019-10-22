
#include "common/common.h"
#include "util/command.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {

  po::options_description desc("Binary execution time");
  desc.add_options()("help", "show help message")(
      "input", po::value<std::string>(), "input file to execute");

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

  std::string filename = vm["input"].as<std::string>();
  std::string command_string("./" + filename);
  LOG(INFO) << command_string;
  auto result = neb::util::command_executor::execute_command(command_string);
  LOG(INFO) << "execution done";
  if (result != 0) {
    LOG(ERROR) << "error: executed by boost::process::system.";
    LOG(ERROR) << "result code = " << result;
    return -1;
  }
  return 0;
}
