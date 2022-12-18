#pragma once
#include "point2d.hh"
#include <vector>
#include <algorithm>
#include <functional>

namespace A {
template <class T>
class Discrete2dCoord {
public:
    typedef Point2d<T> Pn;
    typedef std::vector<Pn> Vec;
    typedef typename Vec::size_type size_type;

    explicit Discrete2dCoord(Vec * v): v{v}, discre{v->size()} {
        auto ds = make_discrete(&Pn::x);
        std::transform(ds.begin(), ds.end(), discre.begin(),
                discre.begin(), [](T x, Pn const & p) { return Pn{x, p.y}; });

        ds = make_discrete(&Pn::y);
        std::transform(ds.begin(), ds.end(), discre.begin(),
                discre.begin(), [](T y, Pn const & p) { return Pn{p.x, y}; });

        sort_v(std::less<Pn>{}, xh, xi).swap(sorted_xy);
        sort_v(typename Pn::YXLess{}, yh, yi).swap(sorted_yx);
    }

    Vec const & discreted() { return discre; }
    Vec const & sorted_x() { return sorted_xy; }
    Vec const & sorted_y() { return sorted_yx; }

    std::vector<size_type> const & x_order() { return xh; }
    std::vector<size_type> const & y_order() { return yh; }

    std::vector<size_type> const & x_index() { return xi; }
    std::vector<size_type> const & y_index() { return yi; }

private:
    template <class Cmp>
    Vec sort_v(Cmp cmp, std::vector<size_type> & ord, std::vector<size_type> & ind) {
        Vec s;
        auto n = v->size();
        s.reserve(n);
        std::copy(v->begin(), v->end(), std::back_inserter(s));
        std::sort(s.begin(), s.end(), cmp);

        auto f = [&](Pn const & p) {
            return std::lower_bound(s.begin(), s.end(), p, cmp) - s.begin();
        };
        ord.clear();
        ord.reserve(n);
        std::transform(v->begin(), v->end(), std::back_inserter(ord), f);

        std::vector<size_type>(n).swap(ind);
        size_type i = 0;
        std::for_each(ord.begin(), ord.end(), [&](size_type o) { ind[o] = i++; });
        return s;
    }

    template <class Extractor>
    std::vector<T> make_discrete(Extractor ext) {
        auto n = v->size();
        std::vector<T> dx;
        dx.reserve(n);
        auto f = std::bind(ext, std::placeholders::_1);
        std::transform(v->begin(), v->end(), std::back_inserter(dx), f);
        std::sort(dx.begin(), dx.end());
        dx.erase(std::unique(dx.begin(), dx.end()), dx.end());
        auto g = [&](Pn const & p) {
            auto x = f(p);
            return std::lower_bound(dx.begin(), dx.end(), x) - dx.begin();
        };
        std::vector<T> res;
        res.reserve(n);
        std::transform(v->begin(), v->end(), std::back_inserter(res), g);
        return res;
    }

    Vec * v;
    Vec discre, sorted_xy, sorted_yx;
    std::vector<size_type> xh, yh, xi, yi;
};
}
