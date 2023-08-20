#include "ext_gcd.hh"
#include "common/assert.hh"

void chk(int a, int b, int gg) {
  auto [g, i, j] = ext_gcd(a, b);
  std::cout << g << ' ' << i << ' ' << j << std::endl;
  ASSERT_EQ(a % g, 0);
  ASSERT_EQ(b % g, 0);
  ASSERT_EQ(g, gg);
  ASSERT_EQ(i * a + j * b, g);
}

int main() {
  chk(4, 3, 1);
  chk(99, 121, 11);
  return 0;
}
