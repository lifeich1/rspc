#include "bfs.hh"
#include "sparse_net.hh"
#include "network01trait.hh"
#include "common/assert.hh"
#include <vector>
#include <iterator>

struct V {
    int c = 0;
    std::size_t s = 0;

    V() {}
    explicit V(std::size_t s): s{s} {}

    V visit(std::size_t, A::Network01Trait::edge_type) {
        V v = *this;
        v.c ++;
        return v;
    }
};

struct TB {
    typedef A::SparseNet<5, 10, A::Network01Trait> net_type;
    typedef V visitor_type;
};

template <std::size_t N>
using S = A::Bfs<N, TB>;

int main() {
    typedef TB::net_type Nw;
    S<5> s;
    {
        Nw nw;
        std::vector<int> e{1, 102, 304, 401};
        std::for_each(e.begin(), e.end(), [&](int i) { nw.arrow(i / 100, i % 100); });

        std::vector<int> src{0, 3};
        s.start(src.begin(), src.end(), nw);

        std::vector<int> v;
        auto const & t = s.get_vis();
        std::transform(t.begin(), t.end(), std::back_inserter(v), std::bind(&V::c, std::placeholders::_1));
        std::copy(v.begin(), v.end(), std::ostream_iterator<std::size_t>(std::cout, " "));
        std::cout << std::endl;
        ASSERT_EQ(v, (std::vector<int>{0, 1, 2, 0, 1}));

        std::vector<std::size_t> vs;
        std::transform(t.begin(), t.end(), std::back_inserter(vs), std::bind(&V::s, std::placeholders::_1));
        ASSERT_EQ(vs, (std::vector<std::size_t>{0, 0, 0, 3, 3}));

        auto const & t1 = s.get_used();
        std::for_each(t1.begin(), t1.end(), [](bool b) { ASSERT_EQ(b, true); });

        s.start(src.begin() + 1, src.end(), nw);

        ASSERT_EQ(std::vector<bool>(t1.begin(), t1.end()), (std::vector<bool>{false, true, true, true, true}));

        vs.clear(); v.clear();
        std::transform(t.begin(), t.end(), std::back_inserter(v), std::bind(&V::c, std::placeholders::_1));
        ASSERT_EQ(v, (std::vector<int>{0, 2, 3, 0, 1}));
        std::transform(t.begin(), t.end(), std::back_inserter(vs), std::bind(&V::s, std::placeholders::_1));
        ASSERT_EQ(vs, (std::vector<std::size_t>{0, 3, 3, 3, 3}));
    }
    return 0;
}
