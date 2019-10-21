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

#include "common/byte.h"
#include "common/common.h"
#include <iomanip>
#include <sstream>

std::string hex_byte(const std::string &addr_base58) {
  neb::bytes addr_bytes = neb::bytes::from_base58(addr_base58);
  std::string addr = neb::byte_to_string(addr_bytes);
  LOG(INFO) << addr.size();

  std::stringstream ss;
  ss << '{';
  for (std::size_t i = 0; i < addr.size(); i++) {
    uint8_t c = addr[i];
    ss << "0x" << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<int>(c) << ',';
  }
  ss.seekp(-1, std::ios_base::end);
  ss << '}';
  return ss.str();
}

std::string entry_point_exp(const std::string &msg) {
  std::string addr_base58 = "n1KxWR8ycXg7Kb9CPTtNjTTEpvka269PniB";
  auto ret = hex_byte(addr_base58);
  return msg + ret;
}
