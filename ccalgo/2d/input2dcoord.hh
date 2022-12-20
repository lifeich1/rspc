#pragma once

#include "point2d.hh"

#include <vector>
#include <algorithm>
#include <iosfwd>
#include <iterator>

namespace A {
template <class T>
/// Input vector<Point2d> like `Input2dCoord<int>(cin, n) >> v`
class Input2dCoord {
public:
    typedef Point2d<T> Pn;
    typedef std::vector<Pn> Vec;

    template <class Stream, class Size>
    explicit Input2dCoord(Stream &&s, Size n) {
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
