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

#include "common/address.h"
#include "common/configuration.h"
#include "common/int128_conversion.h"
#include "fs/blockchain/account/account_db.h"
#include "fs/blockchain/blockchain_api_test.h"
#include "fs/blockchain/transaction/transaction_db.h"
#include "fs/util.h"
#include "runtime/nr/graph/algo.h"
#include "util/json_parser.h"
#include <string>

struct nr_info_t {
  neb::address_t m_address;
  neb::floatxx_t m_in_outs;
  neb::floatxx_t m_median;
  neb::floatxx_t m_weight;
  neb::floatxx_t m_nr_score;
};

struct rank_params_t {
  int64_t m_a;
  int64_t m_b;
  int64_t m_c;
  int64_t m_d;
  neb::floatxx_t m_theta;
  neb::floatxx_t m_mu;
  neb::floatxx_t m_lambda;
};

using compatible_uint64_t = unsigned long long;
using compatible_int64_t = long long;
using transaction_db_ptr_t = std::unique_ptr<neb::fs::transaction_db>;
using account_db_ptr_t = std::unique_ptr<neb::fs::account_db>;
using nr_ret_type =
    std::tuple<int32_t, std::string, std::vector<std::shared_ptr<nr_info_t>>>;
using nr_float_t = float32;

std::string meta_info_to_json(
    const std::vector<std::pair<std::string, std::string>> &meta_info) {
  boost::property_tree::ptree pt;
  for (auto &ele : meta_info) {
    pt.put(ele.first, ele.second);
  }
  std::string ret;
  neb::util::json_parser::write_json(ret, pt);
  return ret;
}

std::unique_ptr<std::vector<std::vector<neb::fs::transaction_info_t>>>
split_transactions_by_block_interval(
    const std::vector<neb::fs::transaction_info_t> &txs,
    int32_t block_interval = 128) {

  auto ret =
      std::make_unique<std::vector<std::vector<neb::fs::transaction_info_t>>>();

  if (block_interval < 1 || txs.empty()) {
    return ret;
  }

  auto it = txs.begin();
  neb::block_height_t block_first = it->m_height;
  it = txs.end();
  it--;
  neb::block_height_t block_last = it->m_height;

  std::vector<neb::fs::transaction_info_t> v;
  it = txs.begin();
  neb::block_height_t b = block_first;
  while (b <= block_last) {
    neb::block_height_t h = it->m_height;
    if (h < b + block_interval) {
      v.push_back(*it++);
    } else {
      ret->push_back(v);
      v.clear();
      b += block_interval;
    }
    if (it == txs.end()) {
      ret->push_back(v);
      break;
    }
  }
  return ret;
}

void filter_empty_transactions_this_interval(
    std::vector<std::vector<neb::fs::transaction_info_t>> &txs) {
  for (auto it = txs.begin(); it != txs.end();) {
    if (it->empty()) {
      it = txs.erase(it);
    } else {
      it++;
    }
  }
}

neb::rt::transaction_graph_ptr_t build_graph_from_transactions(
    const std::vector<neb::fs::transaction_info_t> &trans) {
  auto ret = std::make_unique<neb::rt::transaction_graph>();

  for (auto ite = trans.begin(); ite != trans.end(); ite++) {
    neb::address_t from = ite->m_from;
    neb::address_t to = ite->m_to;
    neb::wei_t value = ite->m_tx_value;
    int64_t timestamp = ite->m_timestamp;
    ret->add_edge(from, to, value, timestamp);
  }
  return ret;
}

std::unique_ptr<std::vector<neb::rt::transaction_graph_ptr_t>>
build_transaction_graphs(
    const std::vector<std::vector<neb::fs::transaction_info_t>> &txs) {

  std::unique_ptr<std::vector<neb::rt::transaction_graph_ptr_t>> tgs =
      std::make_unique<std::vector<neb::rt::transaction_graph_ptr_t>>();

  for (auto it = txs.begin(); it != txs.end(); it++) {
    auto p = build_graph_from_transactions(*it);
    tgs->push_back(std::move(p));
  }
  return tgs;
}

std::unique_ptr<std::unordered_set<neb::address_t>>
get_normal_accounts(const std::vector<neb::fs::transaction_info_t> &txs) {

  auto ret = std::make_unique<std::unordered_set<neb::address_t>>();

  for (auto it = txs.begin(); it != txs.end(); it++) {
    auto from = it->m_from;
    ret->insert(from);

    auto to = it->m_to;
    ret->insert(to);
  }
  return ret;
}

neb::block_height_t get_max_height_this_block_interval(
    const std::vector<neb::fs::transaction_info_t> &txs) {
  if (txs.empty()) {
    return 0;
  }
  // suppose transactions in height increasing order
  return txs.back().m_height;
}

std::unique_ptr<std::unordered_map<neb::address_t, neb::floatxx_t>>
get_account_balance_median(
    const std::unordered_set<neb::address_t> &accounts,
    const std::vector<std::vector<neb::fs::transaction_info_t>> &txs,
    const account_db_ptr_t &db_ptr) {

  auto ret =
      std::make_unique<std::unordered_map<neb::address_t, neb::floatxx_t>>();
  std::unordered_map<neb::address_t, std::vector<neb::wei_t>> addr_balance_v;

  for (auto it = txs.begin(); it != txs.end(); it++) {
    neb::block_height_t max_height = get_max_height_this_block_interval(*it);
    for (auto ite = accounts.begin(); ite != accounts.end(); ite++) {
      neb::address_t addr = *ite;
      neb::wei_t balance =
          db_ptr->get_account_balance_internal(addr, max_height);
      addr_balance_v[addr].push_back(balance);
    }
  }

  neb::floatxx_t zero =
      softfloat_cast<uint32_t, typename neb::floatxx_t::value_type>(0);
  for (auto it = addr_balance_v.begin(); it != addr_balance_v.end(); it++) {
    std::vector<neb::wei_t> &v = it->second;
    sort(v.begin(), v.end(),
         [](const neb::wei_t &w1, const neb::wei_t &w2) { return w1 < w2; });
    size_t v_len = v.size();
    neb::floatxx_t median = neb::to_float<neb::floatxx_t>(v[v_len >> 1]);
    if ((v_len & 0x1) == 0) {
      auto tmp = neb::to_float<neb::floatxx_t>(v[(v_len >> 1) - 1]);
      median = (median + tmp) / 2;
    }

    neb::floatxx_t normalized_median = db_ptr->get_normalized_value(median);
    ret->insert(
        std::make_pair(it->first, neb::math::max(zero, normalized_median)));
  }

  return ret;
}

neb::floatxx_t f_account_weight(neb::floatxx_t in_val, neb::floatxx_t out_val) {
  neb::floatxx_t pi = neb::math::constants<neb::floatxx_t>::pi();
  neb::floatxx_t atan_val = pi / 2.0;
  if (in_val > 0) {
    atan_val = neb::math::arctan(out_val / in_val);
  }
  auto tmp = neb::math::sin(pi / 4.0 - atan_val);
  return (in_val + out_val) * neb::math::exp((-2.0) * tmp * tmp);
}

std::unique_ptr<std::unordered_map<neb::address_t, neb::floatxx_t>>
get_account_weight(const std::unordered_map<neb::address_t,
                                            neb::rt::in_out_val_t> &in_out_vals,
                   const account_db_ptr_t &db_ptr) {

  auto ret =
      std::make_unique<std::unordered_map<neb::address_t, neb::floatxx_t>>();

  for (auto it = in_out_vals.begin(); it != in_out_vals.end(); it++) {
    neb::wei_t in_val = it->second.m_in_val;
    neb::wei_t out_val = it->second.m_out_val;

    neb::floatxx_t f_in_val = neb::to_float<neb::floatxx_t>(in_val);
    neb::floatxx_t f_out_val = neb::to_float<neb::floatxx_t>(out_val);

    neb::floatxx_t normalized_in_val = db_ptr->get_normalized_value(f_in_val);
    neb::floatxx_t normalized_out_val = db_ptr->get_normalized_value(f_out_val);

    auto tmp = f_account_weight(normalized_in_val, normalized_out_val);
    ret->insert(std::make_pair(it->first, tmp));
  }
  return ret;
}

neb::floatxx_t f_account_rank(int64_t a, int64_t b, int64_t c, int64_t d,
                              neb::floatxx_t theta, neb::floatxx_t mu,
                              neb::floatxx_t lambda, neb::floatxx_t S,
                              neb::floatxx_t R) {
  neb::floatxx_t zero =
      softfloat_cast<uint32_t, typename neb::floatxx_t::value_type>(0);
  neb::floatxx_t one =
      softfloat_cast<uint32_t, typename neb::floatxx_t::value_type>(1);
  auto gamma = neb::math::pow(theta * R / (R + mu), lambda);
  auto ret = zero;
  if (S > 0) {
    ret = (S / (one + neb::math::pow(a / S, one / b))) * gamma;
  }
  return ret;
}

std::unique_ptr<std::unordered_map<neb::address_t, neb::floatxx_t>>
get_account_rank(
    const std::unordered_map<neb::address_t, neb::floatxx_t> &account_median,
    const std::unordered_map<neb::address_t, neb::floatxx_t> &account_weight,
    const rank_params_t &rp) {

  auto ret =
      std::make_unique<std::unordered_map<neb::address_t, neb::floatxx_t>>();

  for (auto it_m = account_median.begin(); it_m != account_median.end();
       it_m++) {
    auto it_w = account_weight.find(it_m->first);
    if (it_w != account_weight.end()) {
      neb::floatxx_t rank_val =
          f_account_rank(rp.m_a, rp.m_b, rp.m_c, rp.m_d, rp.m_theta, rp.m_mu,
                         rp.m_lambda, it_m->second, it_w->second);
      ret->insert(std::make_pair(it_m->first, rank_val));
    }
  }

  return ret;
}

std::vector<std::shared_ptr<nr_info_t>>
get_nr_score(const transaction_db_ptr_t &tdb_ptr,
             const account_db_ptr_t &adb_ptr, const rank_params_t &rp,
             neb::block_height_t start_block, neb::block_height_t end_block) {

  auto start_time = std::chrono::high_resolution_clock::now();
  // transactions in total and account inter transactions
  auto txs_ptr =
      tdb_ptr->read_transactions_from_db_with_duration(start_block, end_block);
  LOG(INFO) << "raw tx size: " << txs_ptr->size();
  auto inter_txs_ptr =
      neb::fs::transaction_db::read_transactions_with_address_type(
          *txs_ptr, NAS_ADDRESS_ACCOUNT_MAGIC_NUM,
          NAS_ADDRESS_ACCOUNT_MAGIC_NUM);
  LOG(INFO) << "account to account: " << inter_txs_ptr->size();

  // graph operation
  auto txs_v_ptr = split_transactions_by_block_interval(*inter_txs_ptr);
  LOG(INFO) << "split by block interval: " << txs_v_ptr->size();

  filter_empty_transactions_this_interval(*txs_v_ptr);
  auto tgs_ptr = build_transaction_graphs(*txs_v_ptr);
  if (tgs_ptr->empty()) {
    return std::vector<std::shared_ptr<nr_info_t>>();
  }
  LOG(INFO) << "we have " << tgs_ptr->size() << " subgraphs.";
  for (auto it = tgs_ptr->begin(); it != tgs_ptr->end(); it++) {
    neb::rt::transaction_graph *ptr = it->get();
    neb::rt::graph_algo::non_recursive_remove_cycles_based_on_time_sequence(
        ptr->internal_graph());
    neb::rt::graph_algo::merge_edges_with_same_from_and_same_to(
        ptr->internal_graph());
  }
  LOG(INFO) << "done with remove cycle.";

  neb::rt::transaction_graph *tg = neb::rt::graph_algo::merge_graphs(*tgs_ptr);
  neb::rt::graph_algo::merge_topk_edges_with_same_from_and_same_to(
      tg->internal_graph());
  LOG(INFO) << "done with merge graphs.";

  // in_out amount
  auto in_out_vals_p =
      neb::rt::graph_algo::get_in_out_vals(tg->internal_graph());
  auto in_out_vals = *in_out_vals_p;
  LOG(INFO) << "done with get in_out_vals";

  // median, weight, rank
  auto accounts_ptr = get_normal_accounts(*inter_txs_ptr);
  LOG(INFO) << "account size: " << accounts_ptr->size();

  std::unordered_map<neb::address_t, neb::wei_t> addr_balance;
  for (auto &acc : *accounts_ptr) {
    auto balance = adb_ptr->get_balance(acc, start_block);
    addr_balance.insert(std::make_pair(acc, balance));
  }
  LOG(INFO) << "done with get balance";
  adb_ptr->set_height_address_val_internal(*txs_ptr, addr_balance);
  LOG(INFO) << "done with set height address";

  auto account_median_ptr =
      get_account_balance_median(*accounts_ptr, *txs_v_ptr, adb_ptr);
  LOG(INFO) << "done with get account balance median";
  auto account_weight_ptr = get_account_weight(in_out_vals, adb_ptr);
  LOG(INFO) << "done with get account weight";

  auto account_median = *account_median_ptr;
  auto account_weight = *account_weight_ptr;
  auto account_rank_ptr = get_account_rank(account_median, account_weight, rp);
  auto account_rank = *account_rank_ptr;
  LOG(INFO) << "account rank size: " << account_rank.size();

  std::vector<std::shared_ptr<nr_info_t>> infos;
  for (auto it = accounts_ptr->begin(); it != accounts_ptr->end(); it++) {
    neb::address_t addr = *it;
    if (in_out_vals.find(addr) != in_out_vals.end() &&
        account_median.find(addr) != account_median.end() &&
        account_weight.find(addr) != account_weight.end() &&
        account_rank.find(addr) != account_rank.end()) {
      auto in_outs = in_out_vals[addr].m_in_val + in_out_vals[addr].m_out_val;
      auto f_in_outs = neb::to_float<neb::floatxx_t>(in_outs);

      auto info = std::shared_ptr<nr_info_t>(
          new nr_info_t({addr, f_in_outs, account_median[addr],
                         account_weight[addr], account_rank[addr]}));
      infos.push_back(info);
    }
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  LOG(INFO) << "time spend: "
            << std::chrono::duration_cast<std::chrono::seconds>(end_time -
                                                                start_time)
                   .count()
            << " seconds";
  return infos;
}

nr_ret_type entry_point_nr_impl(compatible_uint64_t start_block,
                                compatible_uint64_t end_block,
                                neb::version_t version, compatible_int64_t a,
                                compatible_int64_t b, compatible_int64_t c,
                                compatible_int64_t d, nr_float_t theta,
                                nr_float_t mu, nr_float_t lambda) {

  auto nbre_path = neb::configuration::instance().nbre_root_dir();
  auto bc_path =
      neb::fs::join_path(nbre_path, "dummy_db/dummy_default_random_.db");
  neb::fs::bc_storage_session::instance().init(
      bc_path, neb::fs::storage_open_for_readonly);

  std::unique_ptr<neb::fs::blockchain_api_base> pba =
      std::unique_ptr<neb::fs::blockchain_api_base>(
          new neb::fs::blockchain_api_test());
  transaction_db_ptr_t tdb_ptr =
      std::make_unique<neb::fs::transaction_db>(pba.get());
  account_db_ptr_t adb_ptr = std::make_unique<neb::fs::account_db>(pba.get());

  LOG(INFO) << "start block: " << start_block << " , end block: " << end_block;
  rank_params_t rp{a, b, c, d, theta, mu, lambda};

  std::vector<std::pair<std::string, std::string>> meta_info;
  meta_info.push_back(
      std::make_pair("start_height", std::to_string(start_block)));
  meta_info.push_back(std::make_pair("end_height", std::to_string(end_block)));
  meta_info.push_back(std::make_pair("version", std::to_string(version)));

  nr_ret_type ret;
  std::get<0>(ret) = 1;
  std::get<1>(ret) = meta_info_to_json(meta_info);
  std::get<2>(ret) = get_nr_score(tdb_ptr, adb_ptr, rp, start_block, end_block);
  return ret;
}

nr_ret_type entry_point_nr(compatible_uint64_t start_block,
                           compatible_uint64_t end_block) {
  compatible_uint64_t a = 100;
  compatible_uint64_t b = 2;
  compatible_uint64_t c = 6;
  compatible_uint64_t d = -9;
  nr_float_t theta = 1;
  nr_float_t mu = 1;
  nr_float_t lambda = 2;
  return entry_point_nr_impl(start_block, end_block, 1, a, b, c, d, theta, mu,
                             lambda);
}

extern int entry_point_lib(const char *msg);

std::string entry_point_exp(const std::string &msg) {
  entry_point_nr(2, 3);
  neb::fs::bc_storage_session::instance().release();
  return msg;
}

int main() {
  auto ret = entry_point_exp("address rank");
  // std::cout << ret << std::endl;
  return 0;
}
