#pragma once

#include "number_interface.hh"
#include <cstdint>

template <int64_t M> struct modint_impl {
  typedef modint_impl<M> Self;
  typedef int compare_r;
  int64_t v;

  void add(Self const &rhs) {
    // TODO
  }
  void sub(Self const &rhs) {
    // TODO
  }
  void mul(Self const &rhs) {
    // TODO
  }
  void div(Self const &rhs) {
    // TODO
  }

  int cmp(Self const &rhs) const {
    return v == rhs.v ? 0 : (v < rhs.v ? -1 : 1);
  }
};

template <int64_t M> using modint = number_interface<modint_impl<M>>;
