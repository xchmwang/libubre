
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
bool cmp(const pair<int, int> &m, const pair<int, int> &n) {
  if (m.second != n.second)
    return m.second < n.second;
  return m.first < n.first;
}

int b_search(vector<pair<int, int>> &nodes, int left, int right, int n) {
  while (left <= right) {
    int mid = (left + right) >> 1;
    if (nodes[mid].second < n)
      left = mid + 1;
    else if (nodes[mid].second > n)
      right = mid - 1;
    else
      return nodes[mid].first;
  }
  return -1;
}

vector<int> twoSum(vector<int> &nums, int target) {
  vector<pair<int, int>> nodes;
  for (int i = 0; i < nums.size(); ++i)
    nodes.push_back(make_pair(i + 1, nums[i]));
  sort(nodes.begin(), nodes.end(), cmp);

  vector<int> ans;
  for (int i = 0; i < nodes.size() - 1 && nodes[i].second <= target / 2; ++i) {
    int index2 =
        b_search(nodes, i + 1, nodes.size() - 1, target - nodes[i].second);
    if (index2 >= 0) {
      ans.push_back(min(nodes[i].first, index2));
      ans.push_back(max(nodes[i].first, index2));
      break;
    }
  }
  return ans;
}

std::string entry_point_exp(const std::string &msg) {
  std::vector<int> v({3, 2, 1});
  v = twoSum(v, 1);
  unsigned long long ret = v[0];
  return "msg:" + msg + ",ret:" + std::to_string(ret);
}
