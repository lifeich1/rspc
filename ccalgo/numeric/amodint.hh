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
  template <class T> explicit array_modint_impl(T t) : v{t} {}
  int64_t operator[](const int i) const { return v[i]; }

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

#define AMODINT_M(L) std::array<int64_t, L> amodint<L>::M
#define AMODINT_M2(L, R) std::array<int64_t, L> amodint<L, R>::M
