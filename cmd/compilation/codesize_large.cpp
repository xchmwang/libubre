
#include <algorithm>
#include <cstdlib>
#include <queue>
#include <string>
#include <vector>
using namespace std;

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

std::string entry_point_exp(const std::string &msg) {
  std::vector<int> v({3, 2, 1});
  maxArea(v);
  sortedArrayToBST(v);
  maxProduct(v);
  findMin(v);
  majorityElement(v);
  rotate(v, 2);
  productExceptSelf(v);
  unsigned long long ret = v[0];
  return "msg:" + msg + ",ret:" + std::to_string(ret);
}

int main() {
  entry_point_exp("large");
  return 0;
}
