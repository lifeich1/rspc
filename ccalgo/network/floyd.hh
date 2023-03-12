#pragma once

#include <cstring>

namespace A {
/// Edge value: Trait::edge_value(edge_type *)
template <std::size_t N, class Net, class Trait>
struct Floyd {
    typedef typename Trait::edge_value_t edge_value_t;

    Net *net;
    int flag[N][N], flag_cur;
    edge_value_t f[N][N];

    explicit Floyd(Net *net): net{net}, flag_cur{1}
    {
        std::memset(flag, 0, sizeof(int) * N * N);
    }

    inline bool reach(int i, int j) const { return flag[i][j] == flag_cur; }

    void operator() (int n) {
        ++ flag_cur;
        for (int i = 0; i < n; flag[i][i] = flag_cur, f[i][i] = 0, ++i)
            for (auto const & [v, e] : net->edges(i)) {
                flag[i][v] = flag_cur;
                f[i][v] = Trait::edge_value(e);
            }
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                if (flag[i][k] != flag_cur) continue;
                for (int j = 0; j < n; ++j) {
                    if (flag[k][j] != flag_cur) continue;
                    auto t = f[i][k] + f[k][j];
                    if ((flag[i][j] != flag_cur && (flag[i][j] = flag_cur, true))
                            || f[i][j] > t) f[i][j] = t;
                }
            }
        }
    }
};
}
