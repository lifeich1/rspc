#include "sparse_net.hh"
#include "network01trait.hh"
#include "common/assert.hh"
#include <algorithm>
#include <iterator>
#include <vector>

template <std::size_t N, std::size_t M>
using Nw01 = A::SparseNet<N, M, A::Network01Trait>;

int main() {
    typedef std::vector<std::size_t> Nv;
#define P(S, ...) \
        do { \
            Nv v; \
            std::transform(nw.edges(S).begin(), nw.edges(S).end(), std::back_inserter(v), [](auto i) { return i.first; }); \
            std::copy(v.begin(), v.end(), std::ostream_iterator<Nv::value_type>(std::cout, " ")); \
            std::cout << std::endl; \
            std::sort(v.begin(), v.end()); \
            ASSERT_EQ(v, (Nv{__VA_ARGS__})); \
        } while (0)
    {
        Nw01<5, 10> nw;
        nw.arrow(0, 1);
        nw.link(2, 1);
        nw.arrow(2, 0);
        nw.arrow(2, 4);

        P(0, 1);
        P(1, 2);
        P(2, 0, 1, 4);
        P(3);
        P(4);
    }
    return 0;
}
