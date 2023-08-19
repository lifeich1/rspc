#include "numeric/modint.hh"
#include "common/assert.hh"

int main() {
  modint<998244353> a = 2;
  ASSERT_EQ(a.v, 2);
  ASSERT_EQ(a.get_inv().v, 998244354 / 2);

  typedef modint<11> m11;
  m11 b = 3;
  ASSERT_EQ((b * 4).v, 1);
  ASSERT_EQ((m11{5} * b).v, 4);
  ASSERT_EQ((b / 2).v, 7);
  ASSERT_EQ((b + 8).v, 0);
  ASSERT_EQ((b - 8).v, 6);

  return 0;
}
