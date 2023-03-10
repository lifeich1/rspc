#include "tree_shape_array.hh"
#include "common/assert.hh"

int main() {
    {
        A::TreeShapeArray<int, 64> ar;
        ar.reset(50);
        ar.update = [](int & a, bool f, int const & b) {
            if (f) a = b; else a += b;
            return true;
        };
        ar.add = [](int &a, int const & b) { a += b; };
        ar.put(3, 9);
        ar.put(30, 8);
        ASSERT_EQ(ar.sum(10, 0), 9);
        ASSERT_EQ(ar.sum(30, 0), 17);
        ar.put(10, 7);
        ASSERT_EQ(ar.sum(50, 0), 24);
    }
    return 0;
}
