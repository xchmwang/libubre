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
#include "cmd/dummy_neb/generator/genesis_generator.h"

genesis_generator::genesis_generator(generate_block *block, int number,
                                     nas init_value)
    : generator_base(block->get_all_accounts(), block, number, 0),
      m_number(number), m_init_value(init_value) {

  std::shared_ptr<corepb::Account> ret = std::make_shared<corepb::Account>();
  std::initializer_list<neb::byte_t> addr = {
      0x19, 0x57, 0x73, 0x3f, 0x7b, 0x52, 0xad, 0x3a, 0x99,
      0xc5, 0x56, 0x84, 0xb3, 0x9f, 0x4c, 0x31, 0xe0, 0x5c,
      0x35, 0x0c, 0x5d, 0xaa, 0xb0, 0x7e, 0x87, 0x5b};
  ret->set_address(neb::address_to_string(addr));
  std::string balance =
      neb::byte_to_string(neb::wei_to_storage(init_value.wei_value()));
  ret->set_balance(balance);
  m_all_accounts->add_account(ret);
}

genesis_generator::~genesis_generator() {}

std::shared_ptr<corepb::Account> genesis_generator::gen_account() {
  return m_block->gen_user_account(m_init_value);
}
std::shared_ptr<corepb::Transaction> genesis_generator::gen_tx() {
  return nullptr;
}
checker_tasks::task_container_ptr_t genesis_generator::gen_tasks() {
  return nullptr;
}
