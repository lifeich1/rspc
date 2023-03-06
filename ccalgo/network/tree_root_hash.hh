#pragma once
#include <unordered_map>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>

namespace A {
template <class Net>
class TreeRootHash {
public:
    typedef std::size_t hash_t;
    typedef std::size_t vertex_t;

    TreeRootHash(Net * net): net{net} {}

    void clear() {
        h_of_v.clear();
        vs_of_qh.clear();
        v_of_h.clear();
    }

    template <class Index>
    hash_t operator() (Index v) {
        auto it = h_of_v.find(v);
        if (it != h_of_v.end()) return it->second;
        std::vector<hash_t> child;
        children_hash(v, child);
        auto qh = qhash(child);
        auto col = vs_of_qh.find(qh);
        hash_t h = v_of_h.size();
        if (col != vs_of_qh.end()) { // check collision
            std::vector<hash_t> ch2;
            for (auto v2 : col->second) {
                children_hash(v2, ch2);
                if (ch2.size() != child.size()) continue;
                if (std::equal(ch2.begin(), ch2.end(), child.begin())) {
                    h = h_of_v[v2];
                    break;
                }
            }
        }
        if (h == v_of_h.size()) { // new hash
            v_of_h.emplace_back(v);
            vs_of_qh[qh].emplace_back(v);
        }
        return h_of_v[v] = h;
    }

private:
    void children_hash(vertex_t v, std::vector<hash_t> & hs) {
        hs.resize(0);
        std::transform(net->edges(v).begin(),
                net->edges(v).end(),
                std::back_inserter(hs),
                [&](auto const & e) { return (*this)(e.first); });
        sort(hs.begin(), hs.end());
    }

    hash_t qhash(std::vector<hash_t> const & hs) {
        return std::accumulate(
                hs.begin(),
                hs.end(),
                0,
                [](hash_t a, hash_t b) { return a * 197 + b; });
    }

    Net * net;
    std::unordered_map<vertex_t, hash_t> h_of_v;
    std::vector<vertex_t> v_of_h;
    std::map<hash_t, std::vector<vertex_t>> vs_of_qh;
};
}
