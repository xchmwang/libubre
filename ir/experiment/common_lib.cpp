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

#include <algorithm>
#include <chrono>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/thread/shared_lock_guard.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <glog/logging.h>
#include <rocksdb/advanced_options.h>
#include <rocksdb/cache.h>
#include <rocksdb/db.h>
#include <rocksdb/filter_policy.h>
#include <rocksdb/options.h>
#include <rocksdb/slice.h>
#include <rocksdb/table.h>
#include <rocksdb/write_batch.h>

#include "core/ir_warden.h"
#include "core/net_ipc/client/client_driver.h"
#include "core/net_ipc/client/nipc_client.h"
#include "core/net_ipc/server/nipc_server.h"

void nbre_func() {
  std::unique_ptr<::ff::net::net_nervure> m_server;
  ::ff::net::tcp_connection_base_ptr m_conn;
  std::unique_ptr<::ff::net::typed_pkg_hub> m_pkg_hub;
  std::atomic_bool m_got_exception_when_start_ipc;
  std::unique_ptr<neb::core::api_request_timer> m_request_timer;
  std::condition_variable m_start_complete_cond_var;
  std::mutex m_mutex;
  std::unique_ptr<neb::core::ipc_client_watcher> m_client_watcher;
  std::chrono::steady_clock::time_point m_last_heart_beat_time;
  std::unique_ptr<neb::util::timer_loop> m_heart_beat_watcher;

  std::unique_ptr<neb::fs::ir_manager> m_ir_manager;
  neb::util::wakeable_queue<std::shared_ptr<nbre_ir_transactions_req>> m_queue;
}

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

bool isSameTree(TreeNode *p, TreeNode *q) {
  if (!p && !q)
    return true;
  return p && q && p->val == q->val && isSameTree(p->left, q->left) &&
         isSameTree(p->right, q->right);
}

bool is_symmetric_tree(TreeNode *left, TreeNode *right) {
  if (!left && !right)
    return true;
  else if (left && !right)
    return false;
  else if (!left && right)
    return false;
  return left->val == right->val &&
         is_symmetric_tree(left->left, right->right) &&
         is_symmetric_tree(left->right, right->left);
}

bool isSymmetric(TreeNode *root) {
  if (!root)
    return true;
  return is_symmetric_tree(root->left, root->right);
}

vector<vector<int>> levelOrder(TreeNode *root) {
  vector<vector<int>> level_order;
  if (!root)
    return level_order;

  queue<TreeNode *> q;
  q.push(root);
  int head = 0, tail = 1, tmp = tail;
  while (!q.empty()) {
    vector<int> level;
    while (head < tail) {
      TreeNode *tree = q.front();
      q.pop();
      ++head;
      level.push_back(tree->val);
      if (tree->left) {
        q.push(tree->left);
        ++tmp;
      }
      if (tree->right) {
        q.push(tree->right);
        ++tmp;
      }
    }
    level_order.push_back(level);
    tail = tmp;
  }
  return level_order;
}

int maxDepth(TreeNode *root) {
  if (!root)
    return 0;
  return max(maxDepth(root->left), maxDepth(root->right)) + 1;
}

TreeNode *build_tree(vector<int> &preorder, int pre_left, int pre_right,
                     vector<int> &inorder, int in_left, int in_right) {
  if (pre_left > pre_right || in_left > in_right)
    return NULL;
  int fp = in_left;
  while (fp <= in_right && inorder[fp++] != preorder[pre_left])
    ;
  int left_size = fp - 1 - in_left, right_size = in_right - fp + 1;
  TreeNode *root = (TreeNode *)malloc(sizeof(TreeNode));
  root->val = preorder[pre_left];
  root->left = build_tree(preorder, pre_left + 1, pre_left + left_size, inorder,
                          in_left, in_left + left_size - 1);
  root->right = build_tree(preorder, pre_right - right_size + 1, pre_right,
                           inorder, fp, in_right);
  return root;
}

TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder) {
  return build_tree(preorder, 0, preorder.size() - 1, inorder, 0,
                    inorder.size() - 1);
}

TreeNode *arr_to_bst(vector<int> &nums, int left, int right) {
  if (left > right)
    return NULL;
  int mid = (left + right) >> 1;
  TreeNode *root = new TreeNode(nums[mid]);
  root->left = arr_to_bst(nums, left, mid - 1);
  root->right = arr_to_bst(nums, mid + 1, right);
  return root;
}

TreeNode *sortedArrayToBST(vector<int> &nums) {
  if (nums.size() < 1)
    return NULL;
  return arr_to_bst(nums, 0, nums.size() - 1);
}

int maxArea(vector<int> &height) {
  int ans = 0;
  int head = 0, tail = height.size() - 1;
  while (head < tail) {
    ans = max(ans, min(height[head], height[tail]) * (tail - head));
    if (height[head] < height[tail])
      ++head;
    else
      --tail;
  }
  return ans;
}

int maxProduct(vector<int> &nums) {
  // max_p, min_p means max_p and min_p product among the subarrays whose last
  // element is nums[i]
  int max_p = nums[0], min_p = nums[0], ans = nums[0];
  for (int i = 1; i < nums.size(); ++i) {
    int tmp_max = max_p;
    max_p = max(nums[i], max(max_p * nums[i], min_p * nums[i]));
    min_p = min(nums[i], min(tmp_max * nums[i], min_p * nums[i]));
    ans = max(ans, max_p);
  }
  return ans;
}

int b_search(vector<int> &nums) {
  int left = 0, right = nums.size() - 1;
  int ans = 0x3f3f3f3f;
  while (left <= right) {
    int mid = (left + right) >> 1;
    if (nums[mid] > nums[right])
      left = mid + 1;
    else if (nums[mid] < nums[right])
      right = mid - 1;
    else
      --right;
    if (nums[mid] < ans)
      ans = nums[mid];
  }
  return ans;
}

int findMin(vector<int> &nums) { return b_search(nums); }

int majorityElement(vector<int> &nums) {
  int majo_ele, cnt = 0;
  for (int i = 0; i < nums.size(); ++i) {
    if (cnt == 0) {
      majo_ele = nums[i];
      cnt = 1;
    } else if (nums[i] == majo_ele)
      ++cnt;
    else
      --cnt;
  }
  return majo_ele;
}

void rotate(vector<int> &nums, int k) {
  int n = nums.size();
  k = k % n;
  reverse(nums.begin(), nums.begin() + n - k);
  reverse(nums.begin() + n - k, nums.end());
  reverse(nums.begin(), nums.end());
}

int division(long long dividend, int divisor) {
  int sign = 1;
  if ((dividend > 0 && divisor < 0) || (dividend < 0 && divisor > 0))
    sign = -1;
  long long a = abs(dividend), b = abs(divisor);
  int ans = 0;
  for (int i = 31; i >= 0 && a >= b; --i) {
    if ((b << i) <= a) {
      a -= (b << i);
      ans += (1 << i);
    }
  }
  if (sign < 0)
    return 0 - ans;
  return ans;
}

vector<int> productExceptSelf(vector<int> &nums) {
  vector<int> ans;
  if (nums.size() < 1)
    return ans;

  long long product = 1;
  int zero_cnt = 0, zero_index;
  for (int i = 0; i < nums.size(); ++i) {
    if (zero_cnt > 1)
      break;
    if (nums[i] == 0) {
      ++zero_cnt;
      zero_index = i;
    } else
      product *= nums[i];
  }
  if (zero_cnt == 0)
    for (int i = 0; i < nums.size(); ++i)
      ans.push_back(division(product, nums[i]));
  else if (zero_cnt > 1)
    for (int i = 0; i < nums.size(); ++i)
      ans.push_back(0);
  else {
    for (int i = 0; i < nums.size(); ++i) {
      if (i == zero_index)
        ans.push_back(product);
      else
        ans.push_back(0);
    }
  }
  return ans;
}

void get_clang_arguments(const boost::property_tree::ptree &json_root,
                         const std::string &key,
                         std::vector<std::string> &container) {

  auto lambda_fun = [&json_root, &key, &container]() {
    boost::property_tree::ptree node = json_root.get_child(key);
    BOOST_FOREACH (boost::property_tree::ptree::value_type &child_node, node) {
      std::string value = child_node.second.get_value<std::string>();
      container.push_back(value);
    }
  };
}

namespace boost {
enum edge_timestamp_t { edge_timestamp };
enum edge_sort_id_t { edge_sort_id };
enum edge_check_id_t { edge_check_id };

BOOST_INSTALL_PROPERTY(edge, timestamp);
BOOST_INSTALL_PROPERTY(edge, sort_id);
BOOST_INSTALL_PROPERTY(edge, check_id);
} // namespace boost

typedef boost::property<
    boost::edge_weight_t, uint64_t,
    boost::property<boost::edge_timestamp_t, int64_t,
                    boost::property<boost::edge_sort_id_t, int64_t>>>
    edge_property_t;

typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::bidirectionalS,
    boost::property<boost::vertex_name_t, std::string>, edge_property_t>
    internal_graph_t;

typedef typename boost::graph_traits<internal_graph_t>::vertex_descriptor
    vertex_descriptor_t;
typedef typename boost::graph_traits<internal_graph_t>::edge_descriptor
    edge_descriptor_t;

typedef
    typename boost::graph_traits<internal_graph_t>::vertex_iterator viterator_t;
typedef typename boost::graph_traits<internal_graph_t>::in_edge_iterator
    ieiterator_t;
typedef typename boost::graph_traits<internal_graph_t>::out_edge_iterator
    oeiterator_t;

void boost_func() {
  boost::filesystem::path cur_path = boost::filesystem::current_path();
  boost::shared_mutex mutex;
  boost::unique_lock<boost::shared_mutex> _l(mutex);

  internal_graph_t graph;
  vertex_descriptor_t s;
  vertex_descriptor_t v;
  std::vector<edge_descriptor_t> edges;
  std::vector<edge_descriptor_t> ret;
  boost::get(boost::edge_timestamp_t(), graph, edges.back());
  oeiterator_t oei, oei_end;
  for (boost::tie(oei, oei_end) = boost::out_edges(v, graph); oei != oei_end;
       oei++) {
    auto target = boost::target(*oei, graph);
  }
}

void glog_func() {
  FLAGS_log_dir = "./";
  google::InitGoogleLogging("glog_func");
  LOG(INFO) << "glog func";
  std::cout << FLAGS_log_dir << std::endl;
}
void rocksdb_func() {
  rocksdb::DB *db = nullptr;
  rocksdb::Options options;
  options.keep_log_file_num = 1;
  rocksdb::BlockBasedTableOptions table_options;
  rocksdb::Status status = rocksdb::DB::Open(options, "rocksdb", &db);
  status = db->Close();
}

int entry_point_lib(const char *msg) {
  std::map<int, int> m;
  m.insert(std::make_pair(1, 1));
  std::unordered_map<int, int> um;
  um.insert(std::make_pair(1, 1));
  std::set<int> s;
  s.insert(1);
  std::unordered_set<int> us;
  us.insert(1);
  std::queue<int> q;
  q.push(1);
  std::deque<int> dq;
  dq.push_back(1);
  std::list<int> l;
  l.size();
  std::tuple<int> tp;
  std::get<0>(tp);
  std::stack<int> st;
  st.push(1);
  auto sptr = std::make_shared<int>();
  std::stringstream ss;
  std::ofstream ofs;
  std::ifstream ifs;
  std::thread t;
  t.get_id();
  std::this_thread::sleep_for(std::chrono::microseconds(1));
  ss << std::hex << std::setw(2) << std::setfill('0')
     << static_cast<int>(msg[0]);
  ofs << msg;
  std::function<void(void)> f;
  if (false) {
    throw std::exception();
  }
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<> dis(0, 10);

  return 0;
}
