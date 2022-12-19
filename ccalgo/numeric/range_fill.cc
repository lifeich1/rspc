#include "range_fill.hh"

#include "common/assert.hh"

#include <vector>
#include <iterator>

int main() {
    typedef std::vector<int> Vec;

    {
        Vec v(4);

        A::range_fill(v.begin(), v.end(), 0);
        ASSERT_EQ(v, (Vec{0, 1, 2, 3}));

        A::range_fill(v.begin(), v.end(), 9990);
        ASSERT_EQ(v, (Vec{9990, 9991, 9992, 9993}));
    }

    {
        Vec v;
        A::range_fill_n(std::back_inserter(v), 4, 0);
        std::cout << v.size() << std::endl;
        ASSERT_EQ(v.size(), 4UL);
        ASSERT_EQ(v, (Vec{0, 1, 2, 3}));
    }

    {
        Vec v;
        A::range_fill_n(std::back_inserter(v), 5, 9990);
        ASSERT_EQ(v, (Vec{9990, 9991, 9992, 9993, 9994}));
    }
    return 0;
}
