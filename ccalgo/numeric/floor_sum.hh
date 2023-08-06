#pragma once

/// floor_sum(a,b,c,n) = sum{(a*i+b) div c | i in 0..=n}
template <typename T> T floor_sum(T a, T b, T c, T n) {
  T r = 0;
  r += (a / c) * n * (n + 1) / 2;
  r += (b / c) * (n + 1);
  a %= c, b %= c;
  if (a == 0)
    return r;
  T m = (a * n + b) / c;
  r += m * n;
  return r - floor_sum<T>(c, c - b - 1, a, m - 1);
}
