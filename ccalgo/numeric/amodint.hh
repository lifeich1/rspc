#pragma once

#include <array>
#include <cstdint>

#include "number_interface.hh"

template <std::size_t L, int ResId = 0> struct array_modint_impl {
  typedef array_modint_impl<L> Self;
  typedef int compare_r;
  static std::array<int64_t, L> M;
  std::array<int64_t, L> v;

  array_modint_impl() { std::fill(v.begin(), v.end(), 0); }
  template <class T> explicit array_modint_impl(T t) {
    std::fill(v.begin(), v.end(), t);
    if (t < 0) {
      for (unsigned i = 0; i < L; ++i)
        v[i] += M[i];
    }
  }
  int64_t operator[](const int i) const { return v[i]; }
  static Self one() { return Self(1); }

  int cmp(Self const &rhs) const {
    for (std::size_t i = 0; i < L; ++i)
      if (v[i] != rhs[i])
        return v[i] < rhs[i] ? -1 : 1;
    return 0;
  }

  void add(Self const &rhs) {
    for (std::size_t i = 0; i < L; ++i)
      v[i] = (v[i] + rhs[i]) % M[i];
  }
  void sub(Self const &rhs) {
    for (std::size_t i = 0; i < L; ++i)
      v[i] = (v[i] + M[i] - rhs[i]) % M[i];
  }
  void mul(Self const &rhs) {
    for (std::size_t i = 0; i < L; ++i)
      v[i] = (v[i] * rhs[i]) % M[i];
  }
  void inv() {
    for (std::size_t i = 0; i < L; ++i)
      v[i] = qpow64(v[i], M[i] - 2, M[i]);
  }
};

template <std::size_t L, int ResId = 0>
using amodint = number_interface<array_modint_impl<L, ResId>>;
#define MINT_M(L, ...)                                                         \
  template <> std::array<int64_t, L> array_modint_impl<L>::M = {__VA_ARGS__}
#define EXMINT_M(L, RID, ...)                                                  \
  template <>                                                                  \
  std::array<int64_t, L> array_modint_impl<L, RID>::M = {__VA_ARGS__}
