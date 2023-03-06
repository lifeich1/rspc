#pragma once
#include <array>

namespace A {
template <std::size_t N, class Net>
class PostorderTravel {
public:
    typedef std::size_t vertex_t;

    explicit PostorderTravel(Net * net): net{net}, tree{nullptr}, cur{0} {
        std::fill(flag.begin(), flag.end(), cur);
    }

    void tree_memory(Net * net) { tree = net; }

    template <class Index, class OutputIt>
    void operator() (Index src, OutputIt d_first) {
        stk[0] = src;
        int p = 0;
        flag[src] = ++cur;
        while (p >= 0) {
            const auto lp = p;
            auto x = stk[p];
            for (auto const & [v, e] : net->edges(x)) {
                if (cur != flag[v]) {
                    flag[v] = cur;
                    stk[++p] = v;
                    if (tree) tree->arrow(x, v);
                }
            }
            if (lp == p) {
                --p;
                *(d_first++) = x;
            }
        }
    }
private:
    Net * net;
    Net * tree;
    std::array<int, N> flag;
    std::array<vertex_t, N> stk;
    int cur;
};
}
