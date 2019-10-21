
#include <string>

#define maxn 10000010
unsigned long long dp[maxn];

unsigned long long fab(unsigned long long n) {
  if (n == 0 || n == 1) {
    return 1;
  }
  if (dp[n]) {
    return dp[n];
  }
  dp[n] = fab(n - 1) + fab(n - 2);
  return dp[n];
}

std::string entry_point_exp(const std::string &msg) {
  unsigned long long ret = fab(91);
  return "msg:" + msg + ",ret:" + std::to_string(ret);
}
