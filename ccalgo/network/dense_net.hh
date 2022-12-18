#pragma once
#include <array>
#include <algorithm>
#include <cstring>

namespace A {
template <std::size_t N, class Trait>
class DenseNet {
public:
    typedef typename Trait::edge_type edge_type;

    DenseNet() { clear(); }

    void clear() {
        std::fill(a.begin(), a.end(), Trait::EDGE_DEFAULT);
    }

    template <class Index>
    inline void arrow(Index src, Index dst, edge_type const & v) {
        a[ei(src, dst)] = v;
    }
    template <class Index>
    inline void arrow(Index src, Index dst) {
        this->arrow(src, dst, Trait::EDGE_SET_VALUE);
    }

    template <class Index>
    inline void link(Index p0, Index p1, edge_type const & v) {
        this->arrow(p0, p1, v);
        this->arrow(p1, p0, v);
    }
    template <class Index>
    inline void link(Index p0, Index p1) {
        this->link(p0, p1, Trait::EDGE_SET_VALUE);
    }

    class EdgeIter {
    public:
        typedef std::pair<std::size_t, edge_type *> value_type;

        value_type const & operator*() const { return p; }
        const value_type * operator->() const { return &p; }

        EdgeIter & operator++() { nx(); return *this; }
        EdgeIter operator++(int) { EdgeIter t = *this; nx(); return t; }

        bool operator==(EdgeIter const & other) const { return p.second == other.p.second; }
        bool operator!=(EdgeIter const & other) const { return p.second != other.p.second; }
    private:
        friend class ::A::DenseNet<N, Trait>::EdgeList;

        EdgeIter(edge_type * p, edge_type * ed)
            : p{p - (ed - N), p}, ed{ed}
        {
            if (*p == Trait::EDGE_DEFAULT) nx();
        }

        void nx() {
            if (p.second != ed) {
                do {
                    ++p.second, ++p.first;
                } while (p.second != ed && *p.second == Trait::EDGE_DEFAULT);
            }
        }

        value_type p;
        edge_type *ed;
    };

    class EdgeList {
    public:
        typedef EdgeIter Iter;
        Iter begin() const { return bg; }
        Iter end() const { return ed; }
    private:
        friend class DenseNet;

        explicit EdgeList(edge_type * bg)
            : bg(bg, bg + N), ed(bg + N, bg + N) {}

         Iter bg, ed;
    };

    template <class Index>
    EdgeList edges(Index i) {
        return EdgeList(&a[ei(i, 0)]);
    }

private:
    template <class Index>
    inline static std::size_t ei(Index src, Index dst) {
        return src * N + dst;
    }

    std::array<edge_type, N * N> a;
};

} // namespace A
