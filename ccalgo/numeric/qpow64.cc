#include "numeric/qpow64.hh"

#include "common/assert.hh"

int main() {
  ASSERT_EQ(qpow64(2, 998244353 - 1, 998244353), 1);
  ASSERT_EQ(qpow64(666, 998244353 - 1, 998244353), 1);
  ASSERT_EQ(qpow64(1023, 998244353 - 1, 998244353), 1);
  return 0;
}
