#pragma once
#include <algorithm>
#include <array>

namespace A {
template <std::size_t N, std::size_t M, class Trait>
class SparseNet {
    struct sto_type;
public:
    typedef typename Trait::edge_type edge_type;

    explicit SparseNet(): a_used{0}, g_used{0} {
        std::fill(g.begin(), g.end(), -1);
    }

    void clear() {
        a_used = 0;
        std::fill(g.begin(), g.begin() + g_used + 1, -1);
        g_used = 0;
    }

    template <class Index>
    void arrow(Index src, Index dst, edge_type const & v) {
        a[a_used].st(dst, v, g[src]);
        g[src] = a_used++;
        std::size_t _s = src;
        if (_s > g_used) g_used = _s;
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

    template <class IterTraits> class EdgeList;

    template <class IterTraits>
    class EdgeIter {
    public:
        typedef typename IterTraits::edge_type edge_type;
        typedef typename IterTraits::sto_type sto_type;
        typedef std::pair<std::size_t, edge_type *> value_type;
        typedef std::forward_iterator_tag iterator_category;
        typedef value_type & reference;
        typedef int difference_type;

        value_type const & operator*() const { return gp<edge_type>(); }
        const value_type * operator->() const { return &p->p; }

        EdgeIter & operator++() { nx(); return *this; }
        EdgeIter operator++(int) { EdgeIter t = *this; nx(); return t; }

        bool operator==(EdgeIter const & other) const { return i == other.i; }
        bool operator!=(EdgeIter const & other) const { return i != other.i; }
    private:
        friend class EdgeList<IterTraits>;

        EdgeIter(sto_type *a, int i): i{i}, p{i >= 0 ? a + i : a}, a{a} {}

        template <typename Edge, std::enable_if_t<!std::is_const_v<Edge>, int> = 0>
        value_type const & gp() const { return p->p; }

        template <typename Edge, std::enable_if_t<std::is_const_v<Edge>, int> = 0>
        value_type const & gp() const { return p->pc; }

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

    template <class IterTraits>
    class EdgeList {
    public:
        typedef typename IterTraits::sto_type sto_type;
        typedef EdgeIter<IterTraits> Iter;
        Iter begin() const { return Iter(a, g); }
        Iter end() const { return Iter(a, -1); }
    private:
        friend class SparseNet;

        EdgeList(sto_type *a, int g): g{g}, a{a} {}

        const int g;
        sto_type * const a;
    };

    struct iter_traits {
        typedef typename SparseNet::edge_type edge_type;
        typedef typename SparseNet::sto_type sto_type;
    };
    struct const_iter_traits {
        typedef typename SparseNet::edge_type const edge_type;
        typedef typename SparseNet::sto_type const sto_type;
    };

    typedef EdgeList<iter_traits> edge_slist;
    typedef EdgeList<const_iter_traits> const_edge_slist;
    typedef typename edge_slist::Iter::value_type iter_output;
    typedef typename const_edge_slist::Iter::value_type const_iter_output;

    template <class Index>
    edge_slist edges(Index i) {
        return edge_slist(a.data(), g[i]);
    }

    template <class Index>
    const_edge_slist edges(Index i) const {
        return const_edge_slist(a.data(), g[i]);
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
    std::size_t a_used, g_used;
};
} // namespace A
