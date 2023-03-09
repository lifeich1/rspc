#include <iostream>

#define POSTORDER_TRAVEL__S_TREEM
#define POSTORDER_TRAVEL__S_PAPA

#include "sparse_net.hh"
#include "network01trait.hh"
#include "postorder_travel.hh"
#include "common/assert.hh"

#include <iterator>

template <std::size_t N>
using Nw01 = A::SparseNet<N, N * 3, A::Network01Trait>;

template <std::size_t N>
using PT = A::PostorderTravel<N, Nw01<N>>;

int main() {
    typedef Nw01<9> Nw9;
    typedef PT<9> PT9;
    typedef std::vector<std::size_t> Nv;
    {
        std::vector<std::size_t> pa;
        Nw9 nw, tnw;
        nw.link(0, 1);
        nw.link(1, 2);
        nw.link(2, 3);
        nw.link(3, 4);
        nw.link(4, 5);

        PT9 pt{&nw, &tnw, &pa};
        Nv v;
        pt(0, std::back_inserter(v));
        std::copy(v.begin(), v.end(), std::ostream_iterator<std::size_t>(std::cout, " "));
        std::cout << std::endl;
        std::copy(pa .begin(), pa .end() , std::ostream_iterator<std::size_t>(std::cout, " "));
        std::cout << std::endl;
        ASSERT_EQ(v, Nv({5, 4, 3, 2, 1, 0}));
        ASSERT_EQ(pa, Nv({0, 0, 1, 2, 3, 4}));
        ASSERT_EQ(tnw.edges(5).begin(), tnw.edges(5).end());
        v.clear();
        std::transform(tnw.edges(0).begin(), tnw.edges(0).end(),
                std::back_inserter(v),
                [&](auto e) { return e.first; });
        ASSERT_EQ(v, Nv({1}));
    }
    {
        Nw9 nw, tnw;
        Nv pa;
        nw.link(1, 5);
        nw.link(1, 6);
        nw.link(1, 2);
        nw.link(2, 3);
        nw.link(2, 4);

        PT9 pt{&nw, &tnw, &pa};
        Nv v;
        pt(1, std::back_inserter(v));
        std::copy(v.begin(), v.end(), std::ostream_iterator<std::size_t>(std::cout, " "));
        std::cout << std::endl;
        ASSERT_EQ(v, Nv({5, 6, 3, 4, 2, 1}));
        ASSERT_EQ(pa, Nv({0, 1, 1, 2, 2, 1, 1}));
        std::copy(pa .begin(), pa .end() , std::ostream_iterator<std::size_t>(std::cout, " "));
        std::cout << std::endl;
        ASSERT_EQ(tnw.edges(5).begin(), tnw.edges(5).end());
        v.clear();
        std::transform(tnw.edges(2).begin(), tnw.edges(2).end(),
                std::back_inserter(v),
                [&](auto e) { return e.first; });
        ASSERT_EQ(v, Nv({3, 4}));
    }
    return 0;
}
