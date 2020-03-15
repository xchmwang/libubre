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
#include "common/common.h"
#include "common/math/internal/math_extension.h"
#include "runtime/nr/graph/graph.h"
#include <stack>
#include <string>

using namespace neb::rt;

void remove_a_cycle(
    transaction_graph::internal_graph_t &graph,
    const std::vector<transaction_graph::edge_descriptor_t> &edges) {

  neb::wei_t min_w = -1;
  for (auto it = edges.begin(); it != edges.end(); it++) {
    neb::wei_t w = boost::get(boost::edge_weight_t(), graph, *it);
    min_w = (min_w == -1 ? w : neb::math::min(min_w, w));
  }

  for (auto it = edges.begin(); it != edges.end(); it++) {
    neb::wei_t w = boost::get(boost::edge_weight_t(), graph, *it);
    boost::put(boost::edge_weight_t(), graph, *it, w - min_w);
    if (w == min_w) {
      boost::remove_edge(*it, graph);
    }
  }
}

void dfs_find_a_cycle_from_vertex_based_on_time_sequence(
    const transaction_graph::internal_graph_t &graph,
    const transaction_graph::vertex_descriptor_t &s,
    const transaction_graph::vertex_descriptor_t &v,
    const std::unordered_set<transaction_graph::vertex_descriptor_t> &dead_v,
    bool &has_cycle,
    std::unordered_map<transaction_graph::vertex_descriptor_t, bool> &visited,
    std::vector<transaction_graph::edge_descriptor_t> &edges,
    std::vector<transaction_graph::edge_descriptor_t> &ret) {

  auto in_time_order =
      [&graph, &edges](const transaction_graph::oeiterator_t &oei) -> bool {
    if (!edges.empty()) {
      int64_t ts = boost::get(boost::edge_timestamp_t(), graph, edges.back());
      int64_t ts_next = boost::get(boost::edge_timestamp_t(), graph, *oei);
      if (ts >= ts_next) {
        return false;
      }
    }
    return true;
  };
  auto exit_cond = [&has_cycle, &edges,
                    &ret](const transaction_graph::oeiterator_t &oei) {
    edges.push_back(*oei);
    has_cycle = true;

    for (auto it = edges.begin(); it != edges.end(); it++) {
      ret.push_back(*it);
    }
  };

  transaction_graph::oeiterator_t oei, oei_end;
  for (boost::tie(oei, oei_end) = boost::out_edges(v, graph); oei != oei_end;
       oei++) {
    if (has_cycle) {
      return;
    }
    auto target = boost::target(*oei, graph);
    if (dead_v.find(target) != dead_v.end()) {
      continue;
    }

    if (target == s) {
      if (!in_time_order(oei)) {
        continue;
      }
      exit_cond(oei);
      return;
    }

    if (visited[target]) {
      if (!in_time_order(oei)) {
        continue;
      }

      for (auto it = edges.begin(); it != edges.end();) {
        auto t = boost::target(*it, graph);
        it = edges.erase(it);
        if (t == target) {
          break;
        }
      }

      exit_cond(oei);
      return;
    }

    visited[target] = true;

    if (edges.empty() || (!edges.empty() && in_time_order(oei))) {
      edges.push_back(*oei);
      dfs_find_a_cycle_from_vertex_based_on_time_sequence(
          graph, s, target, dead_v, has_cycle, visited, edges, ret);
      edges.pop_back();
    }

    visited[target] = false;
  }
  return;
}

std::vector<transaction_graph::edge_descriptor_t>
find_a_cycle_from_vertex_based_on_time_sequence(
    const transaction_graph::internal_graph_t &graph,
    const transaction_graph::vertex_descriptor_t &v,
    const std::unordered_set<transaction_graph::vertex_descriptor_t> &dead_v) {

  std::vector<transaction_graph::edge_descriptor_t> ret;
  std::vector<transaction_graph::edge_descriptor_t> edges;
  std::unordered_map<transaction_graph::vertex_descriptor_t, bool> visited;
  bool has_cycle = false;

  visited[v] = true;
  dfs_find_a_cycle_from_vertex_based_on_time_sequence(
      graph, v, v, dead_v, has_cycle, visited, edges, ret);
  return ret;
}

std::vector<transaction_graph::edge_descriptor_t>
find_a_cycle_based_on_time_sequence(
    const transaction_graph::internal_graph_t &graph,
    const std::unordered_set<transaction_graph::vertex_descriptor_t> &dead_v) {
  std::vector<transaction_graph::edge_descriptor_t> ret;

  std::vector<transaction_graph::vertex_descriptor_t> to_visit;
  transaction_graph::viterator_t vi, vi_end;
  for (boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; vi++) {
    if (dead_v.find(*vi) == dead_v.end()) {
      to_visit.push_back(*vi);
    }
  }

  for (auto it = to_visit.begin(); it != to_visit.end(); it++) {
    auto ret =
        find_a_cycle_from_vertex_based_on_time_sequence(graph, *it, dead_v);
    if (!ret.empty()) {
      return ret;
    }
  }
  return ret;
}

void bfs_decrease_graph_edges(
    const transaction_graph::internal_graph_t &graph,
    const std::unordered_set<transaction_graph::vertex_descriptor_t> &dead_v,
    std::unordered_set<transaction_graph::vertex_descriptor_t> &tmp_dead,
    std::unordered_map<transaction_graph::vertex_descriptor_t, size_t> &dead_to,
    std::unordered_map<transaction_graph::vertex_descriptor_t, size_t>
        &to_dead) {

  std::queue<transaction_graph::vertex_descriptor_t> q;

  auto update_dead_to = [&graph, &dead_v, &tmp_dead, &dead_to](
                            const transaction_graph::vertex_descriptor_t &v) {
    transaction_graph::oeiterator_t oei, oei_end;
    for (boost::tie(oei, oei_end) = boost::out_edges(v, graph); oei != oei_end;
         oei++) {
      auto target = boost::target(*oei, graph);
      if (dead_v.find(target) == dead_v.end() &&
          tmp_dead.find(target) == tmp_dead.end()) {
        if (dead_to.find(target) != dead_to.end()) {
          dead_to[target]++;
        } else {
          dead_to.insert(std::make_pair(target, 1));
        }
      }
    }
  };
  auto update_to_dead = [&graph, &dead_v, &tmp_dead, &to_dead](
                            const transaction_graph::vertex_descriptor_t &v) {
    transaction_graph::ieiterator_t iei, iei_end;
    for (boost::tie(iei, iei_end) = boost::in_edges(v, graph); iei != iei_end;
         iei++) {
      auto source = boost::source(*iei, graph);
      if (dead_v.find(source) == dead_v.end() &&
          tmp_dead.find(source) == tmp_dead.end()) {
        if (to_dead.find(source) != to_dead.end()) {
          to_dead[source]++;
        } else {
          to_dead.insert(std::make_pair(source, 1));
        }
      }
    }
  };

  for (auto &v : tmp_dead) {
    q.push(v);
    update_dead_to(v);
    update_to_dead(v);
  }

  while (!q.empty()) {
    auto &v = q.front();
    q.pop();

    transaction_graph::oeiterator_t oei, oei_end;
    for (boost::tie(oei, oei_end) = boost::out_edges(v, graph); oei != oei_end;
         oei++) {
      auto target = boost::target(*oei, graph);

      if (dead_v.find(target) == dead_v.end() &&
          tmp_dead.find(target) == tmp_dead.end()) {
        auto ret = boost::in_degree(target, graph);
        if (ret && dead_to.find(target) != dead_to.end() &&
            ret == dead_to[target]) {
          q.push(target);
          tmp_dead.insert(target);
          update_dead_to(target);
        }
      }
    }

    transaction_graph::ieiterator_t iei, iei_end;
    for (boost::tie(iei, iei_end) = boost::in_edges(v, graph); iei != iei_end;
         iei++) {
      auto source = boost::source(*iei, graph);

      if (dead_v.find(source) == dead_v.end() &&
          tmp_dead.find(source) == tmp_dead.end()) {
        auto ret = boost::out_degree(source, graph);
        if (ret && to_dead.find(source) != to_dead.end() &&
            ret == to_dead[source]) {
          q.push(source);
          tmp_dead.insert(source);
          update_to_dead(source);
        }
      }
    }
  }
}

bool decrease_graph_edges(
    const transaction_graph::internal_graph_t &graph,
    std::unordered_set<transaction_graph::vertex_descriptor_t> &dead_v,
    std::unordered_map<transaction_graph::vertex_descriptor_t, size_t> &dead_to,
    std::unordered_map<transaction_graph::vertex_descriptor_t, size_t>
        &to_dead) {

  std::unordered_set<transaction_graph::vertex_descriptor_t> tmp_dead;
  transaction_graph::viterator_t vi, vi_end;
  for (boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; vi++) {
    if (dead_v.find(*vi) == dead_v.end()) {
      auto ins = boost::in_degree(*vi, graph);
      auto outs = boost::out_degree(*vi, graph);
      if (!ins || !outs) {
        tmp_dead.insert(*vi);
      }
    }
  }
  bfs_decrease_graph_edges(graph, dead_v, tmp_dead, dead_to, to_dead);
  for (auto &tmp : tmp_dead) {
    dead_v.insert(tmp);
  }
  return boost::num_vertices(graph) != dead_v.size();
}

void remove_cycles_based_on_time_sequence(
    transaction_graph::internal_graph_t &graph) {

  std::vector<transaction_graph::edge_descriptor_t> ret;
  std::unordered_set<transaction_graph::vertex_descriptor_t> dead_v;
  std::unordered_map<transaction_graph::vertex_descriptor_t, size_t> dead_to;
  std::unordered_map<transaction_graph::vertex_descriptor_t, size_t> to_dead;

  while (true) {
    if (!decrease_graph_edges(graph, dead_v, dead_to, to_dead)) {
      break;
    }
    ret = find_a_cycle_based_on_time_sequence(graph, dead_v);
    if (ret.empty()) {
      break;
    }
    remove_a_cycle(graph, ret);
  }
}

void non_recursive_remove_cycles_based_on_time_sequence(
    transaction_graph::internal_graph_t &graph) {
  remove_cycles_based_on_time_sequence(graph);
}

extern int entry_point_lib(const char *msg);

std::string entry_point_exp(const std::string &msg) {
  neb::rt::transaction_graph tg;
  tg.add_edge(neb::to_address("a"), neb::to_address("a"), 1, 1);
  tg.add_edge(neb::to_address("a"), neb::to_address("b"), 2, 2);
  tg.add_edge(neb::to_address("b"), neb::to_address("c"), 3, 3);
  tg.add_edge(neb::to_address("c"), neb::to_address("a"), 4, 4);
  non_recursive_remove_cycles_based_on_time_sequence(tg.internal_graph());
  entry_point_lib(msg.c_str());
  return msg;
}

int main() {
  auto ret = entry_point_exp("decycle");
  // std::cout << ret << std::endl;
  return 0;
}
