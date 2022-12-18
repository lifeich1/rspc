#pragma once
#include <algorithm>
#include <array>

namespace A {
template <std::size_t N, std::size_t M, class Trait>
class SparseNet {
    struct sto_type;
public:
    typedef typename Trait::edge_type edge_type;
    typedef std::pair<std::size_t, edge_type *> iter_output;
    typedef std::pair<std::size_t, edge_type const *> const_iter_output;

    explicit SparseNet(): a_used{0} {
        std::fill(g.begin(), g.end(), -1);
    }

    template <class Index>
    void arrow(Index src, Index dst, edge_type const & v) {
        a[a_used].st(dst, v, g[src]);
        g[src] = a_used++;
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
        typedef iter_output value_type;

        value_type const & operator*() const { return p->p; }
        const value_type * operator->() const { return &p->p; }

        EdgeIter & operator++() { nx(); return *this; }
        EdgeIter operator++(int) { EdgeIter t = *this; nx(); return t; }

        bool operator==(EdgeIter const & other) const { return i == other.i; }
        bool operator!=(EdgeIter const & other) const { return i != other.i; }
    private:
        friend class ::A::SparseNet<N, M, Trait>::EdgeList;

        EdgeIter(sto_type *a, int i): i{i}, p{i >= 0 ? a + i : a}, a{a} {}

        void nx() {
            if (i >= 0) {
                i = p->nx_sto;
                if (i >= 0) {
                    p = a + i;
                }
            }
        }

        int i;
        sto_type *p, * const a;
    };

    class ConstEdgeIter {
    public:
        typedef const_iter_output value_type;

        value_type const & operator*() const { return p->pc; }
        const value_type * operator->() const { return &p->pc; }

        ConstEdgeIter & operator++() { nx(); return *this; }
        ConstEdgeIter operator++(int) { ConstEdgeIter t = *this; nx(); return t; }

        bool operator==(ConstEdgeIter const & other) const { return i == other.i; }
        bool operator!=(ConstEdgeIter const & other) const { return i != other.i; }
    private:
        friend class ::A::SparseNet<N, M, Trait>::ConstEdgeList;

        ConstEdgeIter(sto_type const * a, int i): i{i}, p{i >= 0 ? a + i : a}, a{a} {}

        void nx() {
            if (i >= 0) {
                i = p->nx_sto;
                if (i >= 0) {
                    p = a + i;
                }
            }
        }

        int i;
        sto_type const * p;
        sto_type const * const a;
    };

    class EdgeList {
    public:
        typedef EdgeIter Iter;
        Iter begin() const { return EdgeIter(a, g); }
        Iter end() const { return EdgeIter(a, -1); }
    private:
        friend class SparseNet;

        EdgeList(sto_type *a, int g): g{g}, a{a} {}

        const int g;
        sto_type * const a;
    };

    class ConstEdgeList {
    public:
        typedef ConstEdgeIter Iter;
        Iter begin() const { return Iter(a, g); }
        Iter end() const { return Iter(a, -1); }
    private:
        friend class SparseNet;

        ConstEdgeList(sto_type const * a, int g): g{g}, a{a} {}

        const int g;
        sto_type const * const a;
    };

    template <class Index>
    EdgeList edges(Index i) {
        return EdgeList(a.data(), g[i]);
    }

    template <class Index>
    ConstEdgeList edges(Index i) const {
        return ConstEdgeList(a.data(), g[i]);
    }

private:
    struct sto_type{
        edge_type v;
        int nx_sto;
        iter_output p;
        const_iter_output pc;

        void st(std::size_t dst, edge_type const & v, int nx) {
            this->v = v;
            p.first = dst;
            p.second = &this->v;
            pc = p;
            nx_sto = nx;
        }
    };

    std::array<sto_type, M> a;
    std::array<int, N> g;
    std::size_t a_used;
};
} // namespace A
