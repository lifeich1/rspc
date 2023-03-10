#include "vary_tuple.hh"
#include "assert.hh"

int main() {
    {
        std::tuple<int, int64_t> t1 = {1, 2};
        std::tuple<int, int64_t> t2 = {10, 20};
        std::tuple<int, int64_t> t3 = {11, 22};
        ASSERT_EQ((A::vary_tuple(t1) + t2).t, t3);
        ASSERT_EQ((A::vary_tuple(t1) + std::move(t2)).t, t3);
    }
    return 0;
}
