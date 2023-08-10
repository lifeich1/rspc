#pragma once

#include <array>
#include <cstdint>

#include "number_interface.hh"

template <std::size_t L> struct array_modint_impl {
  typedef array_modint_impl<L> Self;
  typedef bool compare_r;
  static std::array<int64_t, L> M;
  std::array<int64_t, L> v;

  bool eq(Self const &rhs) const {
    for (int i = 0; i < v.size(); ++i) {
      if (v[i] != rhs.v[i])
        return false;
    }
    return true;
  }

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
};

template <std::size_t L> using amodint = number_interface<array_modint_impl<L>>;
