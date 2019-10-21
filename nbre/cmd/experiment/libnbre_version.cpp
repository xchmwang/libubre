// Copyright (C) 2018 go-nebulas authors
//
// This file is part of the go-nebulas library.
//
// the go-nebulas library is free software: you can redistribute it and/or
// modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// the go-nebulas library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the go-nebulas library.  If not, see
// <http://www.gnu.org/licenses/>.
//

#include "common/common.h"
#include "common/version.h"
#include <string>

std::string entry_point_exp(const std::string &msg) {
  neb::version v;
  v.major_version() = 0;
  v.minor_version() = 1;
  v.patch_version() = 1;
  return msg + std::to_string(v.data());
}

int main() {
  auto ret = entry_point_exp("version");
  std::cout << ret << std::endl;
  return 0;
}
