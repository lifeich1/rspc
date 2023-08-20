#pragma once

#include <cstdint>
#include <tuple>

template <typename T = int64_t> std::tuple<T, T, T> ext_gcd(T a, T b) {
  T ia[] = {1, 0, 0, 0}, ib[] = {0, 1, 0, 0}, a1, b1, d;
  do {
    if (0 == b)
      return {a, ia[0], ib[0]};
    d = a / b;
    a1 = b, b1 = a - d * b;
    ia[2] = ia[1], ib[2] = ib[1];
    ia[3] = ia[0] - d * ia[1], ib[3] = ib[0] - d * ib[1];
    if (0 == b1)
      return {a1, ia[2], ib[2]};
    d = a1 / b1;
    a = b1, b = a1 - d * b1;
    ia[0] = ia[3], ib[0] = ib[3];
    ia[1] = ia[2] - d * ia[3], ib[1] = ib[2] - d * ib[3];
  } while (1);
  return {a, 1, 0};
}
