
#include <fstream>
#include <string>

unsigned long long fab(unsigned long long n) {
  if (n == 0 || n == 1) {
    return 1;
  }
  return fab(n - 1) + fab(n - 2);
}

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
  unsigned long long ret = fab(43);
  file_io();
  return "msg:" + msg + ",ret:" + std::to_string(ret);
}

int main() {
  entry_point_exp("computation_io");
  return 0;
}
