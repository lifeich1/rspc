#pragma once

#include "point2d.hh"

#include <vector>
#include <algorithm>
#include <iosfwd>
#include <iterator>

namespace A {
template <class T>
class Input2dCoord {
public:
    typedef Point2d<T> Pn;
    typedef std::vector<Pn> Vec;

    template <class CharT, class Size>
    explicit Input2dCoord(std::basic_istream<CharT> &&s, Size n) {
        v.reserve(n);
        std::copy_n(std::istream_iterator<Pn>(s), n, std::back_inserter(v));
    }

    Vec & operator>>(Vec & taker) {
        taker.swap(v);
        return taker;
    }

private:
    Vec v;
};
}
