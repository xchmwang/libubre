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

#include <string>
#include <tuple>
#include <vector>

typedef std::string name_t;
typedef uint64_t height_t;

typedef std::tuple<name_t, height_t, height_t> row_t;

std::vector<row_t> entry_point_auth() {

  std::vector<row_t> auth_table = {std::make_tuple("nr", 1ULL, 200000ULL),
                                   std::make_tuple("exp", 1ULL, 200000ULL),
                                   std::make_tuple("lib", 1ULL, 200000ULL),
                                   std::make_tuple("dip", 1ULL, 200000ULL)};
  return auth_table;
}

