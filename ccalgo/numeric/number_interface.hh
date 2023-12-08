#pragma once

#include <type_traits>
#include <utility>

template <typename Base> struct number_interface : public Base {
  typedef number_interface<Base> Self;

  template <typename... Args>
  number_interface(Args... args) : Base(std::forward<Args>(args)...) {}

  Self get_inv() const {
    Self r = *this;
    r.inv();
    return r;
  }
  void div(Self const &rhs) { this->mul(rhs.get_inv()); }

  template <class T> Self qpow(T k) {
    Self r = Self::one();
    if (k <= 0)
      return r;
    Self t = *this;
    for (; k > 0; k >>= 1, t *= t)
      if (k & 1)
        r *= t;
    return r;
  }
  template <class T> Self &pow(T k) {
    *this = this->qpow(k);
    return *this;
  }
  Self &operator+=(Self const &other) {
    this->add(other);
    return *this;
  }
  Self &operator*=(Self const &other) {
    this->mul(other);
    return *this;
  }
  Self &operator/=(Self const &other) {
    this->div(other);
    return *this;
  }
  Self &operator-=(Self const &other) {
    this->sub(other);
    return *this;
  }

  Self operator+(Self const &other) const {
    Self r{*this};
    r += other;
    return r;
  }
  Self operator-(Self const &other) const {
    Self r{*this};
    r -= other;
    return r;
  }
  Self operator-() const {
    Self r;
    r -= *this;
    return r;
  }
  Self operator*(Self const &other) const {
    Self r{*this};
    r *= other;
    return r;
  }
  Self operator/(Self const &other) const {
    Self r{*this};
    r /= other;
    return r;
  }

  bool operator<(Self const &rhs) const { return this->cmp(rhs) < 0; }
  bool operator>(Self const &rhs) const { return this->cmp(rhs) > 0; }
  bool operator==(Self const &rhs) const { return this->cmp(rhs) == 0; }
  bool operator<=(Self const &rhs) const { return this->cmp(rhs) == 0; }
  bool operator>=(Self const &rhs) const { return this->cmp(rhs) >= 0; }
};
