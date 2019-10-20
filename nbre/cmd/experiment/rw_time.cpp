
#include "common/byte.h"
#include "fs/rocksdb_storage.h"
#include <boost/program_options.hpp>
#include <fstream>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {

  po::options_description desc("Experiment read and write");
  desc.add_options()("help", "show help message")(
      "db_path", po::value<std::string>(), "Database file directory")(
      "input", po::value<std::string>(), "data file");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }

  if (!vm.count("db_path")) {
    std::cout << "You must specify \"db_path\"!" << std::endl;
    return 1;
  }
  if (!vm.count("input")) {
    std::cout << "You must specify \"input\"!" << std::endl;
    return 1;
  }

  std::string db_path = vm["db_path"].as<std::string>();
  std::string filename = vm["input"].as<std::string>();

  std::ifstream ifs;
  ifs.open(filename, std::ios::in | std::ios::binary);
  if (!ifs.is_open())
    return -1;
  ifs.seekg(0, ifs.end);
  std::ifstream::pos_type size = ifs.tellg();
  neb::bytes buf(size);
  ifs.seekg(0, ifs.beg);
  ifs.read((char *)buf.value(), buf.size());
  ifs.close();

  neb::fs::rocksdb_storage rs;
  rs.open_database(db_path, neb::fs::storage_open_for_readwrite);
  LOG(INFO) << "start to write db";
  rs.put(filename, buf);
  LOG(INFO) << "write db done";
  LOG(INFO) << "start to read db";
  rs.get(filename);
  LOG(INFO) << "read db done";
  rs.close_database();
  return 0;
}
