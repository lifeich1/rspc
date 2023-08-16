#pragma once

#include "number_interface.hh"
#include "qpow64.hh"
#include <cstdint>

template <int64_t M> struct modint_impl {
  typedef modint_impl<M> Self;
  typedef int compare_r;
  int64_t v;

  modint_impl() : v{0} {}
  template <class T> explicit modint_impl(T t) : v{t} {}

  void add(Self const &rhs) { v = (v + rhs.v) % M; }
  void sub(Self const &rhs) { v = (v + M - rhs.v) % M; }
  void mul(Self const &rhs) { v = (v * rhs.v) % M; }
  void inv() { v = qpow64(v, M - 2, M); }

  int cmp(Self const &rhs) const {
    return v == rhs.v ? 0 : (v < rhs.v ? -1 : 1);
  }
};

template <int64_t M> using modint = number_interface<modint_impl<M>>;
