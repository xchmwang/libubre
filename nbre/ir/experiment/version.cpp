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

#include "common/version.h"
#include "common/address.h"
#include "common/common.h"
#include <string>

struct nr_info_t {
  neb::address_t m_address;
  neb::floatxx_t m_in_outs;
  neb::floatxx_t m_median;
  neb::floatxx_t m_weight;
  neb::floatxx_t m_nr_score;
};

using nr_ret_type =
    std::tuple<int32_t, std::string, std::vector<std::shared_ptr<nr_info_t>>>;
using compatible_uint64_t = unsigned long long;

extern nr_ret_type entry_point_nr(compatible_uint64_t start_block,
                                  compatible_uint64_t end_block);

std::string entry_point_exp(const std::string &msg) {
  neb::version v;
  v.major_version() = 0;
  v.minor_version() = 1;
  v.patch_version() = 1;
  return msg + std::to_string(v.data());
}
