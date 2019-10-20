
#include <fstream>
#include <string>

void file_io() {
  std::ofstream ofs;
  ofs.open("io_file");
  std::string s(10000, 'x');
  std::string tmp;
  for (int i = 0; i < 10000; i++) {
    ofs << s;
  }
  ofs.close();
}

std::string entry_point_exp(const std::string &msg) {
  file_io();
  return "msg:" + msg;
}

int main() {
  entry_point_exp("io");
  return 0;
}
