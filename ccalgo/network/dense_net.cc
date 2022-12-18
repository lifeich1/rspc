#include "dense_net.hh"
#include "network01trait.hh"
#include "common/assert.hh"
#include <algorithm>
#include <iterator>
#include <vector>

template <std::size_t N>
using Nw01 = A::DenseNet<N, A::Network01Trait>;

int main() {
    typedef std::vector<std::size_t> Nv;
    {
        Nw01<5> nw;
        nw.arrow(0, 1);
        nw.link(2, 1);
        nw.arrow(2, 0);
        nw.arrow(2, 4);

        {
            Nv v;
            std::transform(nw.edges(0).begin(), nw.edges(0).end(), std::back_inserter(v), [](auto i) { return i.first; });
            std::copy(v.begin(), v.end(), std::ostream_iterator<Nv::value_type>(std::cout, " "));
            std::cout << std::endl;
            ASSERT_EQ(v, (Nv{1}));
        }

#define P(S, ...) \
        do { \
            Nv v; \
            std::transform(nw.edges(S).begin(), nw.edges(S).end(), std::back_inserter(v), [](auto i) { return i.first; }); \
            std::copy(v.begin(), v.end(), std::ostream_iterator<Nv::value_type>(std::cout, " ")); \
            std::cout << std::endl; \
            ASSERT_EQ(v, (Nv{__VA_ARGS__})); \
        } while (0)

        P(1, 2);
        P(2, 0, 1, 4);
        P(3);
        P(4);

#define Q(S, ...) \
        do { \
            auto const & cnw = nw; Nv v; \
            std::transform(cnw.edges(S).begin(), cnw.edges(S).end(), std::back_inserter(v), [](auto i) { return i.first; }); \
            std::copy(v.begin(), v.end(), std::ostream_iterator<Nv::value_type>(std::cout, " ")); \
            std::cout << std::endl; \
            ASSERT_EQ(v, (Nv{__VA_ARGS__})); \
        } while (0)

        Q(0, 1);
        Q(1, 2);
        Q(2, 0, 1, 4);
        Q(3);
        Q(4);
    }
    return 0;
}
