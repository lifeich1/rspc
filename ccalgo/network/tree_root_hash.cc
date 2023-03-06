#include "dense_net.hh"
#include "network01trait.hh"
#include "tree_root_hash.hh"
#include "common/assert.hh"

#include <iterator>

template <std::size_t N>
using Nw01 = A::DenseNet<N, A::Network01Trait>;

template <class Net>
using THash = A::TreeRootHash<Net>;

int main() {
    typedef Nw01<9> Nw9;
    typedef THash<Nw9> THash9;
    typedef typename THash9::hash_t hash_t;
    typedef std::vector<hash_t> Hv;
    {
        Nw9 nw;
        nw.arrow(0, 1);
        nw.arrow(1, 2);
        nw.arrow(2, 3);
        nw.arrow(3, 4);
        nw.arrow(4, 5);

        THash9 thash{&nw};

        std::vector<int> vs{0, 1, 2, 3, 4, 5};
        std::vector<hash_t> hs;
        std::transform(vs.begin(), vs.end(), std::back_inserter(hs), thash);
        ASSERT_EQ(hs, Hv({5, 4, 3, 2, 1, 0}));
    }
    {
        Nw9 nw;
        nw.arrow(1, 5);
        nw.arrow(1, 6);
        nw.arrow(1, 2);
        nw.arrow(2, 3);
        nw.arrow(2, 4);

        THash9 thash{&nw};
        std::vector<int> vs{1, 2, 3, 4, 5, 6};
        std::vector<hash_t> hs;
        std::transform(vs.begin(), vs.end(), std::back_inserter(hs), thash);
        std::copy(hs.begin(), hs.end(), std::ostream_iterator<hash_t>(std::cout, " "));
        std::cout << std::endl;
        ASSERT_EQ(hs, Hv({2, 1, 0, 0, 0, 0}));
    }
    return 0;
}
