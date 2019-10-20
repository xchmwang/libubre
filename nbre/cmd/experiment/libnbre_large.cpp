
#include "common/common.h"
#include "common/configuration.h"
#include "common/int128_conversion.h"
#include "common/nebulas_currency.h"
#include "fs/blockchain/blockchain_api_test.h"
#include "runtime/nr/impl/nebulas_rank.h"
#include "runtime/nr/impl/nr_impl.h"

using nr_float_t = float32;
using version_t = neb::compatible_uint64_t;

neb::rt::nr::nr_ret_type
entry_point_nr_impl(neb::compatible_uint64_t start_block,
                    neb::compatible_uint64_t end_block, version_t version,
                    neb::compatible_int64_t a, neb::compatible_int64_t b,
                    neb::compatible_int64_t c, neb::compatible_int64_t d,
                    nr_float_t theta, nr_float_t mu, nr_float_t lambda) {

  std::unique_ptr<neb::fs::blockchain_api_base> pba;
  if (neb::use_test_blockchain) {
    pba = std::unique_ptr<neb::fs::blockchain_api_base>(
        new neb::fs::blockchain_api_test());
  } else {
    pba = std::unique_ptr<neb::fs::blockchain_api_base>(
        new neb::fs::blockchain_api());
  }
  neb::rt::nr::transaction_db_ptr_t tdb_ptr =
      std::make_unique<neb::fs::transaction_db>(pba.get());
  neb::rt::nr::account_db_ptr_t adb_ptr =
      std::make_unique<neb::fs::account_db>(pba.get());

  LOG(INFO) << "start block: " << start_block << " , end block: " << end_block;
  neb::rt::nr::rank_params_t rp{a, b, c, d, theta, mu, lambda};

  std::vector<std::pair<std::string, std::string>> meta_info;
  meta_info.push_back(
      std::make_pair("start_height", std::to_string(start_block)));
  meta_info.push_back(std::make_pair("end_height", std::to_string(end_block)));
  meta_info.push_back(std::make_pair("version", std::to_string(version)));

  neb::rt::nr::nr_ret_type ret;
  std::get<0>(ret) = 1;
  std::get<1>(ret) = neb::rt::meta_info_to_json(meta_info);
  std::get<2>(ret) = neb::rt::nr::nebulas_rank::get_nr_score(
      tdb_ptr, adb_ptr, rp, start_block, end_block);
  return ret;
}

int main() {
  entry_point_nr_impl(1, 2, 3, 10, 20, 30, 40, 1.0, 2.0, 3.0);
  return 0;
}

