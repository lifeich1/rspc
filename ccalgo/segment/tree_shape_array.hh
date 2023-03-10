#pragma once

#include <array>
#include <type_traits>
#include <functional>

namespace A {

template <class T, std::size_t N>
struct TreeShapeArray {
    std::array<int, N> flag;
    int flag_cur = 0;
    std::array<T, N> a;
    int n; // work at [1, n]

    std::function<bool(T &, bool, T const &)> update; // return true for update executed
    std::function<void(T &, T const &)> add;

    void reset(int n) { ++ flag_cur, this->n = n; }

    void put(int x, T const & v) {
        for (; x <= n; x += (x & -x))
            if (this->update(a[x], flag[x] != flag_cur, v)) flag[x] = flag_cur;
            else break;
    }
    void sum_r(int x, T & res) {
        for (; x > 0; x -= (x & -x))
            if (flag_cur == flag[x]) this->add(res, a[x]);
    }

    T sum(int x, T init) {
        this->sum_r(x, init); return init;
    }
};
}
