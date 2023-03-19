#pragma once
#include <array>

#if 0
#define POSTORDER_TRAVEL__S_TREEM
#define POSTORDER_TRAVEL__S_PAPA
#define POSTORDER_TRAVEL__NOEDGEEXT
#endif

namespace A {
template <std::size_t N, class Net>
class PostorderTravel {
public:
    typedef std::size_t vertex_t;

    explicit PostorderTravel(Net * net
#ifdef POSTORDER_TRAVEL__S_TREEM
            , Net * tree
#endif
#ifdef POSTORDER_TRAVEL__S_PAPA
            , std::vector<std::size_t> * papa
#endif
            ): net{net}
#ifdef POSTORDER_TRAVEL__S_TREEM
            , tree{tree}
#endif
#ifdef POSTORDER_TRAVEL__S_PAPA
            , papa{papa}
#endif
    {
        std::fill(flag.begin(), flag.end(), cur);
    }

#ifdef POSTORDER_TRAVEL__S_PAPA
# define SET_PA(A, B) set_pa(A, B)
#else
# define SET_PA(A, B)
#endif
#ifdef POSTORDER_TRAVEL__S_TREEM
# define TR_AR(A, B) tree->arrow(A, B)
#else
# define TR_AR(A, B)
#endif
    template <class Index, class OutputIt>
    void operator() (Index src, OutputIt d_first) {
        stk[0] = src;
        int p = 0;
        flag[src] = ++cur;
        SET_PA(src, src);
        while (p >= 0) {
            const auto lp = p;
            auto x = stk[p];
#ifdef POSTORDER_TRAVEL__NOEDGEEXT
            for (auto const & v : net->edges(x)) {
#else
            for (auto const & [v, e] : net->edges(x)) {
#endif
                if (cur != flag[v]) {
                    flag[v] = cur;
                    stk[++p] = v;
                    SET_PA(v, x);
                    TR_AR(x, v);
                }
            }
            if (lp == p) {
                --p;
                *(d_first++) = x;
            }
        }
    }
#undef SET_PA
#undef TR_AR
private:
#ifdef POSTORDER_TRAVEL__S_PAPA
    inline void set_pa(vertex_t v, vertex_t pa) {
        if (papa->size() <= v) papa->resize(v + 1);
        papa->at(v) = pa;
    }
#endif

    Net * net;
#ifdef POSTORDER_TRAVEL__S_TREEM
    Net * tree;
#endif
#ifdef POSTORDER_TRAVEL__S_PAPA
    std::vector<vertex_t> * papa;
#endif
    std::array<int, N> flag;
    std::array<vertex_t, N> stk;
    int cur = 0;
};
}
