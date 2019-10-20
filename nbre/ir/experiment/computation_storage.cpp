
#include <string>

unsigned long long fab_computation(unsigned long long n) {
  if (n == 0 || n == 1) {
    return 1;
  }
  return fab_computation(n - 1) + fab_computation(n - 2);
}

#define maxn 10000010
unsigned long long dp[maxn];

unsigned long long fab_storage(unsigned long long n) {
  if (n == 0 || n == 1) {
    return 1;
  }
  if (dp[n]) {
    return dp[n];
  }
  dp[n] = fab_storage(n - 1) + fab_storage(n - 2);
  return dp[n];
}

std::string entry_point_exp(const std::string &msg) {
  unsigned long long ret = fab_computation(43);
  fab_storage(91);
  return "msg:" + msg + ",ret:" + std::to_string(ret);
}
