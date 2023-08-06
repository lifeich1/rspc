#include "numeric/floor_sum.hh"
#include "common/assert.hh"

#include <type_traits>

int64_t fsum(int64_t a, int64_t b, int64_t c, int64_t n) {
  int64_t r = 0;
  for (int i = 0; i <= n; ++i)
    r += (a * i + b) / c;
  return r;
}

void chk(int64_t a, int64_t b, int64_t c, int64_t n) {
  auto out = floor_sum(a, b, c, n);
  static_assert(std::is_same_v<decltype(out), int64_t>);
  auto ans = fsum(a, b, c, n);
  ASSERT_EQ(out, ans);
  std::cout << "out: " << out << ", ans: " << ans << std::endl;
}

int main() {
  chk(2, 3, 4, 5);
  chk(99, 37, 17, 9);
  return 0;
}
