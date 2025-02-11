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

#include "runtime/dip/dip_handler.h"
#include "common/configuration.h"
#include "core/ir_warden.h"
#include "fs/ir_manager/api/ir_api.h"
#include "fs/proto/ir.pb.h"
#include "fs/storage_holder.h"
#include "jit/jit_driver.h"
#include "runtime/dip/dip_reward.h"
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <ff/functionflow.h>

namespace neb {
namespace rt {
namespace dip {

dip_handler::dip_handler() : m_has_curr(false) {
  m_storage = neb::fs::storage_holder::instance().nbre_db_ptr();
}

void dip_handler::check_dip_params(block_height_t height) {

  if (!m_has_curr && m_incoming.empty()) {
    load_storage("dip_rewards", m_dip_reward);
    load_storage("nr_results", m_nr_result, 1 << 4);
    load_storage("nr_sums", m_nr_sum, 1 << 4);
    auto dip_versions_ptr = neb::fs::ir_api::get_ir_versions("dip", m_storage);
    if (dip_versions_ptr->empty()) {
      return;
    }
    LOG(INFO) << "dip versions not empty, size " << dip_versions_ptr->size();

    std::reverse(dip_versions_ptr->begin(), dip_versions_ptr->end());
    for (auto version : *dip_versions_ptr) {
      auto nbre_ir_ptr =
          neb::core::ir_warden::instance().get_ir_by_name_version("dip",
                                                                  version);
      block_height_t available_height = nbre_ir_ptr->height();
      m_incoming.push(std::make_pair(version, available_height));
    }
  }

  std::pair<version_t, block_height_t> tmp;

  while (!m_incoming.empty() && m_incoming.front().second <= height) {
    tmp = m_incoming.front();
    m_incoming.pop();

    try {
      LOG(INFO) << "init dip param list, version: " << tmp.first
                << ", available height " << tmp.second;
      auto dip_ret = run_dip_ir("dip", tmp.first, tmp.second, 0);
      if (!std::get<0>(dip_ret)) {
        auto info = std::make_shared<dip_params_t>();
        info->deserialize_from_string(std::get<1>(dip_ret));
        m_dip_params_list.push_back(info);
      }
    } catch (const std::exception &e) {
      LOG(INFO) << "dip params init failed " << e.what();
    }
  }

  if (tmp.first && tmp.second) {
    m_has_curr = true;
  }
}

void dip_handler::deploy(version_t version, block_height_t available_height) {
  m_incoming.push(std::make_pair(version, available_height));
}

void dip_handler::start(neb::block_height_t height,
                        const dip_params_t *dip_params) {
  check_dip_params(height);
  if (!m_has_curr) {
    // LOG(INFO) << "dip params not init";
    return;
  }

  // get start block and block interval if default
  auto last_ele_ptr = m_dip_params_list.back();
  auto &last_ele = *last_ele_ptr;
  block_height_t dip_start_block = last_ele.get<start_block>();
  block_height_t dip_block_interval = last_ele.get<block_interval>();

  if (dip_params) {
    LOG(INFO) << "dip meta not null";
    dip_start_block = dip_params->get<start_block>();
    dip_block_interval = dip_params->get<block_interval>();
  }

  if (height < dip_start_block + dip_block_interval) {
    LOG(INFO) << "wait to sync";
    return;
  }

  uint64_t interval_nums = (height - dip_start_block) / dip_block_interval;
  uint64_t hash_height = dip_start_block + dip_block_interval * interval_nums;

  if (height != hash_height) {
    return;
  }
  LOG(INFO) << "to start calculate dip reward for hash_height " << hash_height;

  if (m_dip_reward.exist(hash_height)) {
    LOG(INFO) << "dip reward already exists";
    return;
  }

  if (!m_in_process.insert_if_not_exist(hash_height, true)) {
    LOG(INFO) << "dip reward already in processing";
    return;
  }

  // get dip version if default
  std::string dip_name = "dip";
  uint64_t dip_version = 0;
  auto tmp_ptr = neb::core::ir_warden::instance().get_ir_by_name_height(
      dip_name, hash_height, false);
  assert(!tmp_ptr->empty());
  dip_version = tmp_ptr->back().version();
  if (dip_params) {
    dip_version = dip_params->get<p_version>();
  }

  // ff::para<> p;
  // p([this, dip_name, dip_version, hash_height]() {
  try {
    auto dip_ret = run_dip_ir(dip_name, dip_version, hash_height, hash_height);
    if (std::get<0>(dip_ret)) {
      auto dip_str_ptr = dip_reward::dip_info_to_json(dip_ret);
      dump_storage("dip_rewards", hash_height,
                   str_sptr_t{std::move(dip_str_ptr)}, m_dip_reward);
      LOG(INFO) << "dump dip rewards done";

      auto &nr_ret = std::get<3>(dip_ret);
      auto nr_str_ptr = nr::nebulas_rank::nr_info_to_json(nr_ret);
      dump_storage("nr_results", hash_height, str_sptr_t{std::move(nr_str_ptr)},
                   m_nr_result, 1 << 4);
      LOG(INFO) << "dump nr results done";

      auto nr_sum_ptr = nr::nebulas_rank::get_nr_sum_str(nr_ret);
      dump_storage("nr_sums", hash_height, str_sptr_t{std::move(nr_sum_ptr)},
                   m_nr_sum, 1 << 4);
      LOG(INFO) << "dump nr sums done";
    } else {
      LOG(INFO) << std::get<1>(dip_ret);
    }
    } catch (const std::exception &e) {
      LOG(INFO) << "jit driver execute dip failed " << e.what();
    }
    m_in_process.erase(hash_height);
    //});
}

std::shared_ptr<dip_params_t>
dip_handler::get_dip_params_previous(neb::block_height_t height) {

  auto tmp_ptr = std::make_shared<dip_params_t>();
  tmp_ptr->set<start_block>(height);
  auto ret = m_dip_params_list.try_previous(
      tmp_ptr, [](const std::shared_ptr<dip_params_t> &d1,
                  const std::shared_ptr<dip_params_t> &d2) {
        return d1->get<start_block>() < d2->get<start_block>();
      });
  LOG(INFO) << "try previous status " << ret.first;
  assert(ret.first);
  return ret.second;
}

std::string
dip_handler::get_dip_reward_when_missing(neb::block_height_t hash_height,
                                         const dip_params_t &dip_params) {

  LOG(INFO) << "call func get_dip_reward_when_missing";

  auto dip_start_block = dip_params.get<start_block>();
  auto dip_block_interval = dip_params.get<block_interval>();

  if (hash_height < dip_start_block + dip_block_interval) {
    auto it_ptr = get_dip_params_previous(dip_start_block);
    dip_start_block = it_ptr->get<start_block>();
    dip_block_interval = it_ptr->get<block_interval>();

    auto interval_nums = (hash_height - dip_start_block) / dip_block_interval;
    hash_height = dip_start_block + dip_block_interval * interval_nums;
    start(hash_height, it_ptr.get());
    auto ret = std::string("{\"err\":\"dip reward missing, wait to restart\"}");
    LOG(INFO) << ret;
    return ret;
  }

  start(hash_height, &dip_params);
  auto ret = std::string("{\"err\":\"dip reward missing, wait to restart or "
                         "height out of range\"}");
  LOG(INFO) << ret;
  return ret;
}

std::shared_ptr<dip_params_t>
dip_handler::get_dip_params(neb::block_height_t height) {

  LOG(INFO) << "get dip params height " << height;
  auto tmp_ptr = std::make_shared<dip_params_t>();
  tmp_ptr->set<start_block>(height);
  auto ret = m_dip_params_list.try_lower_than(
      tmp_ptr, [](const std::shared_ptr<dip_params_t> &d1,
                  const std::shared_ptr<dip_params_t> &d2) {
        return d1->get<start_block>() < d2->get<start_block>();
      });
  LOG(INFO) << "try lower than status " << ret.first;
  assert(ret.first);
  return ret.second;
}

str_sptr_t dip_handler::get_dip_reward(neb::block_height_t height) {
  LOG(INFO) << "call func get_dip_reward";

  if (!m_has_curr) {
    auto ret = std::string("{\"err\":\"dip params not init yet\"}");
    LOG(INFO) << ret;
    return std::make_shared<std::string>(ret);
  }

  if (!m_dip_params_list.empty()) {
    auto first_ele_ptr = m_dip_params_list.front();
    auto &first_ele = *first_ele_ptr;
    if (height <
        first_ele.get<start_block>() + first_ele.get<block_interval>()) {
      auto ret = boost::str(
          boost::format("{\"err\":\"available height is %1%\"}") %
          (first_ele.get<start_block>() + first_ele.get<block_interval>()));
      LOG(INFO) << ret;
      return std::make_shared<std::string>(ret);
    }
  }

  LOG(INFO) << "dip history size " << m_dip_params_list.size();
  auto it_ptr = get_dip_params(height);
  auto &it = *it_ptr;
  block_height_t dip_start_block = it.get<start_block>();
  block_height_t dip_block_interval = it.get<block_interval>();
  LOG(INFO) << "history start block " << dip_start_block << " , block interval "
            << dip_block_interval;

  uint64_t interval_nums = (height - dip_start_block) / dip_block_interval;
  uint64_t hash_height = dip_start_block + dip_block_interval * interval_nums;
  LOG(INFO) << "mapping height " << height << " to hash_height " << hash_height;

  if (!m_dip_reward.exist(hash_height)) {
    LOG(INFO) << "dip reward not exists";
    auto ret = get_dip_reward_when_missing(hash_height, it);
    return std::make_shared<std::string>(ret);
  }
  LOG(INFO) << "dip reward exists";
  auto ret = m_dip_reward.try_get_val(hash_height);
  assert(ret.first);
  LOG(INFO) << *ret.second;
  return ret.second;
}

str_sptr_t dip_handler::get_nr_result(neb::block_height_t height) {
  LOG(INFO) << "call func get_nr_result height " << height;
  auto ret = m_nr_result.try_lower_than(height);
  LOG(INFO) << "try lower than returned status " << ret.first;
  if (!ret.first) {
    auto ret = std::string("{\"err\":\"no such nr result\"}");
    LOG(INFO) << ret;
    return std::make_shared<std::string>(ret);
  }
  assert(ret.first);
  auto &tmp = ret.second;
  LOG(INFO) << *tmp.second;
  return tmp.second;
}

str_sptr_t dip_handler::get_nr_sum(neb::block_height_t height) {
  LOG(INFO) << "call func get_nr_sum height " << height;
  auto ret = m_nr_sum.try_lower_than(height);
  if (!ret.first) {
    auto ret = std::string("{\"err\":\"no such nr sum\"}");
    LOG(INFO) << ret;
    return std::make_shared<std::string>(ret);
  }
  assert(ret.first);
  auto &tmp = ret.second;
  LOG(INFO) << *tmp.second;
  return tmp.second;
}

void dip_handler::dump_storage(
    const std::string &key, neb::block_height_t hash_height,
    const str_sptr_t &val_ptr,
    thread_safe_map<block_height_t, str_sptr_t> &mem_cache,
    size_t storage_max_size) {
  std::unique_lock<std::mutex> _l(m_mutex);

  auto update_to_storage = [](const std::string &key,
                              const boost::property_tree::ptree &val_pt,
                              neb::fs::rocksdb_storage *rs) {
    std::stringstream ss;
    boost::property_tree::json_parser::write_json(ss, val_pt, false);
    rs->put(key, neb::string_to_byte(ss.str()));
  };

  LOG(INFO) << "call func dump_storage";
  neb::bytes val_bytes;
  try {
    val_bytes = m_storage->get(key);
  } catch (const std::exception &e) {
    LOG(INFO) << key << " empty " << e.what();

    boost::property_tree::ptree ele, arr, root;
    ele.put("", *val_ptr);
    arr.push_back(std::make_pair("", ele));
    root.add_child(key, arr);
    update_to_storage(key, root, m_storage);

    mem_cache.insert(hash_height, val_ptr);
    LOG(INFO) << "insert " << key << "pair height " << hash_height << ", "
              << *val_ptr;
    return;
  }

  LOG(INFO) << key << " not empty";
  boost::property_tree::ptree root;
  std::stringstream ss(neb::byte_to_string(val_bytes));
  boost::property_tree::json_parser::read_json(ss, root);

  boost::property_tree::ptree &arr = root.get_child(key);
  boost::property_tree::ptree ele;
  ele.put("", *val_ptr);
  arr.push_back(std::make_pair("", ele));
  LOG(INFO) << "insert " << key;
  update_to_storage(key, root, m_storage);

  mem_cache.insert(hash_height, val_ptr);
  LOG(INFO) << "insert " << key << " pair height " << hash_height << ", "
            << *val_ptr;

  if (mem_cache.size() > storage_max_size) {
    auto first_ele = mem_cache.begin();
    mem_cache.erase(first_ele.first);
  }
}

void dip_handler::load_storage(
    const std::string &key,
    thread_safe_map<block_height_t, str_sptr_t> &mem_cache,
    size_t storage_max_size) {
  std::unique_lock<std::mutex> _l(m_mutex);

  // LOG(INFO) << "call func load_storage";
  neb::bytes val_bytes;
  try {
    val_bytes = m_storage->get(key);
  } catch (const std::exception &e) {
    // LOG(INFO) << key << " empty " << e.what();
    return;
  }

  LOG(INFO) << key << " not empty";
  boost::property_tree::ptree root;
  std::stringstream ss(neb::byte_to_string(val_bytes));
  boost::property_tree::json_parser::read_json(ss, root);

  BOOST_FOREACH (boost::property_tree::ptree::value_type &v,
                 root.get_child(key)) {
    boost::property_tree::ptree pt = v.second;
    auto record_ptr =
        std::make_shared<std::string>(pt.get<std::string>(std::string()));

    boost::property_tree::ptree tmp_pt;
    std::stringstream ss(*record_ptr);
    boost::property_tree::json_parser::read_json(ss, tmp_pt);
    block_height_t end_height = tmp_pt.get<block_height_t>("end_height");

    auto h = end_height + 1;
    mem_cache.insert(h, record_ptr);
    LOG(INFO) << "insert " << key << " pair height " << h << ", "
              << *record_ptr;

    if (mem_cache.size() > storage_max_size) {
      auto first_ele = mem_cache.begin();
      mem_cache.erase(first_ele.first);
    }
  }
}

dip_ret_type dip_handler::run_dip_ir(const std::string &name, version_t version,
                                     block_height_t ir_height,
                                     block_height_t var_height) {
  std::stringstream ss;
  ss << name << version;
  std::string name_version = ss.str();
  LOG(INFO) << "dip name version " << name_version;

  auto irs_ptr =
      neb::core::ir_warden::instance().get_ir_by_name_height(name, ir_height);
  LOG(INFO) << "dip ir and depends size " << irs_ptr->size();

  jit_driver &jd = jit_driver::instance();
  LOG(INFO) << "jit driver run with " << name_version << ',' << irs_ptr->size()
            << ',' << neb::configuration::instance().dip_func_name() << ','
            << var_height;
  auto dip_ret = jd.run<dip_ret_type>(
      name_version, *irs_ptr, neb::configuration::instance().dip_func_name(),
      var_height);
  LOG(INFO) << "dip reward returned";
  return dip_ret;
}

} // namespace dip
} // namespace rt
} // namespace neb
