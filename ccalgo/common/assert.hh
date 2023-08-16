#pragma once

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

namespace A {
template <class T1, class T2>
void assert_eq(T1 &&lhs, T2 &&rhs, std::string const &mark, int qcode = 1) {
  if (!(lhs == rhs)) {
    // TODO print if streamable
    std::cout << mark << std::endl << std::flush;
    std::quick_exit(qcode);
  }
}

template <class T1, class T2>
void _assert_eq(T1 &&lhs, T2 &&rhs, const char *file, int line,
                std::string const &mark, int qcode = 1) {
  std::stringstream ss;
  ss << file << ":" << line << ": " << mark;
  assert_eq(std::forward<T1>(lhs), std::forward<T2>(rhs), ss.str(), qcode);
}
} // namespace A

#define ASSERT_EQ(L, R)                                                        \
  do {                                                                         \
    A::_assert_eq((L), (R), __FILE__, __LINE__,                                \
                  "found NOT eqaul of " #L ", " #R);                           \
  } while (0)
