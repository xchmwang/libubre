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
#include "fs/proto/ir.pb.h"
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

typedef std::pair<bool, std::unique_ptr<nbre::NBREIR>> ir_ret_type;

ir_ret_type get_ir(const std::string &name, neb::version_t version,
                   neb::fs::storage *rs) {
  ir_ret_type ret_pair;
  ret_pair.second = std::make_unique<nbre::NBREIR>();
  auto &nbre_ir = ret_pair.second;
  neb::bytes nbre_bytes;
  try {
    std::stringstream ss;
    ss << name << version;
    nbre_bytes = rs->get(ss.str());
  } catch (const std::exception &e) {
    LOG(INFO) << "no such ir named " << name << " with version " << version
              << ' ' << e.what();
    ret_pair.first = false;
    return ret_pair;
  }

  bool ret = nbre_ir->ParseFromArray(nbre_bytes.value(), nbre_bytes.size());
  if (!ret) {
    throw std::runtime_error("parse nbre failed");
  }
  ret_pair.first = true;
  return ret_pair;
}

void get_ir_depends(const std::string &name, neb::version_t version,
                    neb::fs::storage *rs,
                    std::vector<std::pair<std::string, neb::version_t>> &irs) {

  std::unordered_set<std::string> s;
  std::queue<std::pair<std::string, neb::version_t>> q;
  q.push(std::make_pair(name, version));
  std::stringstream ss;

  while (!q.empty()) {
    auto &ele = q.front();
    q.pop();
    ss.clear();
    ss << ele.first << ele.second;
    if (s.find(ss.str()) == s.end()) {
      auto ret = get_ir(ele.first, ele.second, rs);
      if (ret.first) {
        for (auto &dep : ret.second->depends()) {
          q.push(std::make_pair(dep.name(), dep.version()));
        }
        irs.push_back(std::make_pair(ele.first, ele.second));
      }
      s.insert(ss.str());
    }
  }
}

void gen_ir(neb::fs::storage *rs) {
  nbre::NBREIR xx_ir;
  std::string xx_name = "xx";
  neb::version_t xx_version = 123;
  std::string yy_name = "yy";
  neb::version_t yy_version = 789;

  xx_ir.set_name(xx_name);
  xx_ir.set_version(xx_version);
  xx_ir.set_height(456);
  auto deps = xx_ir.add_depends();
  deps->set_name(yy_name);
  deps->set_version(yy_version);
  auto size = xx_ir.ByteSizeLong();
  neb::bytes xx_buf(size);
  xx_ir.SerializeToArray((void *)xx_buf.value(), xx_buf.size());

  std::stringstream ss;
  ss << xx_name << xx_version;
  rs->put(ss.str(), xx_buf);

  ss.str("");
  ss.clear();
  nbre::NBREIR yy_ir;
  yy_ir.set_name(yy_name);
  yy_ir.set_version(yy_version);
  yy_ir.set_height(456);

  size = yy_ir.ByteSizeLong();
  neb::bytes yy_buf(size);
  yy_ir.SerializeToArray((void *)yy_buf.value(), yy_buf.size());
  ss << yy_name << yy_version;
  rs->put(ss.str(), yy_buf);
}

std::string entry_point_exp(const std::string &msg) {
  auto ptr = std::make_unique<memory_storage>();
  gen_ir(ptr.get());
  std::vector<std::pair<std::string, neb::version_t>> irs;
  get_ir_depends("xx", 123, ptr.get(), irs);
  std::stringstream ss;
  for (auto &ir : irs) {
    ss << ir.first << ir.second << ',';
  }
  return msg + ss.str();
}

int main() {
  auto ret = entry_point_exp("depend");
  std::cout << ret << std::endl;
  return 0;
}
