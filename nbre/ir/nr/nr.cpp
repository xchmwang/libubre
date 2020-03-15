
#include "runtime/nr/impl/nr_impl.h"

neb::rt::nr::nr_ret_type entry_point_nr(neb::compatible_uint64_t start_block,
                                        neb::compatible_uint64_t end_block) {
  auto to_version_t = [](uint32_t major_version, uint16_t minor_version,
                         uint16_t patch_version) -> neb::rt::nr::version_t {
    return (0ULL + major_version) + ((0ULL + minor_version) << 32) +
           ((0ULL + patch_version) << 48);
  };

  neb::compatible_uint64_t a = 100;
  neb::compatible_uint64_t b = 2;
  neb::compatible_uint64_t c = 6;
  neb::compatible_uint64_t d = -9;
  neb::rt::nr::nr_float_t theta = 1;
  neb::rt::nr::nr_float_t mu = 1;
  neb::rt::nr::nr_float_t lambda = 2;
  return neb::rt::nr::entry_point_nr_impl(start_block, end_block,
                                          to_version_t(0, 0, 1), a, b, c, d,
                                          theta, mu, lambda);
}

