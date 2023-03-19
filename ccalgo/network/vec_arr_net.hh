#pragma once

#include <vector>

namespace A {
template <class T>
struct VecArrNet {
    typedef std::vector<T> vec;
    typedef vec * vec_arr;

    vec_arr arr;

    VecArrNet(vec_arr arr): arr{arr} {}
    template <class Ind> vec & edges(Ind i) { return arr[i]; }
    template <class Ind> vec const & edges(Ind i) const { return arr[i]; }
};

template <class T>
VecArrNet<T> net(std::vector<T> arr[]) { return {arr}; }
}
