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

#include "fs/storage.h"
#include "util/thread_safe_map.h"

class memory_storage : public neb::fs::storage {
public:
  memory_storage();
  virtual ~memory_storage();
  memory_storage(const memory_storage &ms) = delete;
  memory_storage &operator=(const memory_storage &) = delete;

  virtual neb::bytes get_bytes(const neb::bytes &key);
  virtual void put_bytes(const neb::bytes &key, const neb::bytes &val);
  virtual void del_by_bytes(const neb::bytes &key);

  virtual void enable_batch();
  virtual void disable_batch();
  virtual void flush();

protected:
  neb::thread_safe_map<neb::bytes, neb::bytes> m_memory;
};

memory_storage::memory_storage() = default;
memory_storage::~memory_storage() = default;

neb::bytes memory_storage::get_bytes(const neb::bytes &key) {
  auto ret = m_memory.try_get_val(key);
  if (!ret.first) {
    throw std::exception();
  }
  return ret.second;
}

void memory_storage::put_bytes(const neb::bytes &key, const neb::bytes &val) {
  m_memory.insert(key, val);
}

void memory_storage::del_by_bytes(const neb::bytes &key) {
  m_memory.erase(key);
}

void memory_storage::enable_batch() {}
void memory_storage::disable_batch() {}
void memory_storage::flush() {}

std::string entry_point_exp(const std::string &msg) {
  auto ptr = std::make_unique<memory_storage>();
  std::string key = "xx";
  std::string val = "yy";
  neb::bytes b_key = neb::string_to_byte(key);
  neb::bytes b_val = neb::string_to_byte(val);
  ptr->put_bytes(b_key, b_val);
  ptr->get_bytes(b_key);
  return key + msg + val;
}
