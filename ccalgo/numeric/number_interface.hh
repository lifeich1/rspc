#pragma once

#include <type_traits>

template <typename Base> struct number_interface : public Base {
  typedef number_interface<Base> Self;

  template <class T> number_interface(T const &t) : Base{t} {}
  template <class T> number_interface(T &&t) : Base{t} {}

  void get_inv() const {
    Self r = *this;
    r.inv();
    return r;
  }
  void div(Self const &rhs) { mul(rhs.inv()); }

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
  using enable_cmp =
      std::enable_if<std::is_same_v<typename Base::compare_r, int>, int>;
  template <enable_cmp = 1> bool operator<(Self const &rhs) const {
    return this->cmp(rhs) < 0;
  }
  template <enable_cmp = 1> bool operator>(Self const &rhs) const {
    return this->cmp(rhs) > 0;
  }
  template <enable_cmp = 1> bool operator==(Self const &rhs) const {
    return this->cmp(rhs) == 0;
  }
  template <enable_cmp = 1> bool operator<=(Self const &rhs) const {
    return this->cmp(rhs) <= 0;
  }
  template <enable_cmp = 1> bool operator>=(Self const &rhs) const {
    return this->cmp(rhs) >= 0;
  }
  using enable_eq =
      std::enable_if<std::is_same_v<typename Base::compare_r, bool>, int>;
  template <enable_eq = 1> bool operator==(Self const &rhs) const {
    return this->eq(rhs);
  }
};
