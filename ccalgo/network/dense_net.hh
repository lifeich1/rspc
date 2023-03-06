#pragma once
#include <array>
#include <algorithm>
#include <cstring>

// Net describe facility
//  - edges(): get slist-like one node edges travel facility
//  - *edges()::Iter = std::pair(dst_node_index, &edge_desc)
//  - link(p0, p1[, v]): add bidirectional edge
//  - arrow(src, dst[, v]): add directional edge

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

    template <class Edge> class EdgeListImpl;

    template <class Edge>
    class EdgeIterImpl {
    public:
        typedef EdgeIterImpl type;

        typedef std::pair<std::size_t, Edge *> value_type;
        typedef std::forward_iterator_tag iterator_category;
        typedef value_type & reference;
        typedef int difference_type;
        typedef type pointer;

        value_type const & operator*() const { return p; }
        const value_type * operator->() const { return &p; }

        type & operator++() { nx(); return *this; }
        type operator++(int) { type t = *this; nx(); return t; }

        bool operator==(type const & other) const { return p.second == other.p.second; }
        bool operator!=(type const & other) const { return p.second != other.p.second; }
    private:
        friend class ::A::DenseNet<N, Trait>::EdgeListImpl<Edge>;

        EdgeIterImpl(Edge * p, Edge * ed)
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
        Edge * const ed;
    };

    template <class Edge>
    class EdgeListImpl {
    public:
        typedef EdgeIterImpl<Edge> Iter;
        Iter begin() const { return bg; }
        Iter end() const { return ed; }
    private:
        friend class DenseNet;

        explicit EdgeListImpl(Edge * bg)
            : bg(bg, bg + N), ed(bg + N, bg + N) {}

         const Iter bg, ed;
    };

    typedef EdgeListImpl<edge_type> edge_slist;
    typedef EdgeListImpl<const edge_type> const_edge_slist;
    typedef typename edge_slist::Iter::value_type iter_output;
    typedef typename const_edge_slist::Iter::value_type const_iter_output;

    template <class Index>
    edge_slist edges(Index i) {
        return edge_slist(&a[ei(i, static_cast<Index>(0))]);
    }

    template <class Index>
    const_edge_slist edges(Index i) const {
        return const_edge_slist(&a[ei(i, static_cast<Index>(0))]);
    }

private:
    template <class Index>
    inline static std::size_t ei(Index src, Index dst) {
        return src * N + dst;
    }

    std::array<edge_type, N * N> a;
};

} // namespace A
