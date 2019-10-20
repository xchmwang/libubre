
#include "runtime/version.h"

neb::version get_version() {
  neb::version v;
  v.major_version() = 0;
  v.minor_version() = 1;
  v.patch_version() = 1;
  return v;
}

int main() {
  get_version();
  return 0;
}
