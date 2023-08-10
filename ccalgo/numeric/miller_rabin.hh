#pragma once

#include "calc_in_mod.hh"
#include <vector>

namespace A {
template <class T = int64_t> struct MillerRabinPrimeTest {
  std::vector<T> test = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

  bool operator()(T const p) const {
    for (auto y : test)
      if (1 != qpow_in_mod(y, p - 1, p))
        return false;
    return true;
  }
};
} // namespace A

template <class T = int64_t> A::MillerRabinPrimeTest<T> fast_prime_tester() {
  return {};
}
