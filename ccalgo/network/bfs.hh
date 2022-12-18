#pragma once
#include <array>
#include <algorithm>
#include <functional>

namespace A {
template <std::size_t N, class Trait>
class Bfs {
public:
    typedef typename Trait::net_type net_type;
    typedef typename net_type::const_iter_output edge_iter_output;
    typedef typename Trait::visitor_type visitor_type; // new_visitor = visitor{}.visit(cur_node, edge_v), start: visitor(src)
    typedef std::array<visitor_type, N> visitor_array;
    typedef std::array<bool, N> used_array;

    Bfs() {}

    visitor_array const & get_vis() { return vis; }
    used_array const & get_used() { return used; }

    template <class InputIt, class Size = std::size_t>
    void start(InputIt first, InputIt last, net_type const & nw, Size node_limit = N) {
        p = 0; q = last - first;
        std::copy(first, last, ns.begin());
        std::fill(used.begin(), used.begin() + std::min(node_limit, N), false);
        std::for_each(ns.begin(), ns.begin() + q, [&](std::size_t i) { used[i] = true; vis[i] = visitor_type(i); });

        auto fvis = [&](std::size_t pre, edge_iter_output e) {
            auto d = e.first;
            if (!used[d]) {
                used[d] = true;
                ns[q++] = d;
                vis[d] = vis[pre].visit(d, *e.second);
            }
        };
        for (; p < q; ++p) {
            auto x = ns[p];
            auto es = nw.edges(x);
            std::for_each(es.begin(), es.end(), std::bind(fvis, x, std::placeholders::_1));
        }
    }

private:
    visitor_array vis;
    used_array used;
    std::array<std::size_t, N> ns;
    std::size_t p, q;
};
} // namespace A
