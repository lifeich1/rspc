#include "discrete2dcoord.hh"

#include "common/assert.hh"

#include <iterator>

int main() {
    typedef typename A::Discrete2dCoord<int> D2C;
    typedef typename D2C::Pn Pn;
    typedef typename D2C::Vec Vec;
    typedef std::vector<typename D2C::size_type> Ivec;

    {
        Vec v{Pn(1111, 2222), Pn(2222, 1111), Pn(2222, 2222), Pn(1111, 1111), Pn(1111, 11111)};
        D2C d{&v};

        std::copy(d.discreted().begin(),
                d.discreted().end(),
                std::ostream_iterator<Pn>(std::cout, "\n"));

        ASSERT_EQ(d.discreted(), (Vec{Pn(0, 1), Pn(1, 0), Pn(1, 1), Pn(0, 0), Pn(0, 2)}));
        ASSERT_EQ(d.sorted_x(), (Vec{Pn(1111, 1111), Pn(1111, 2222), Pn(1111, 11111), Pn(2222, 1111), Pn(2222, 2222)}));
        ASSERT_EQ(d.sorted_y(), (Vec{Pn(1111, 1111), Pn(2222, 1111), Pn(1111, 2222), Pn(2222, 2222), Pn(1111, 11111)}));

        ASSERT_EQ(d.x_order(), (Ivec{1, 3, 4, 0, 2}));
        ASSERT_EQ(d.y_order(), (Ivec{2, 1, 3, 0, 4}));
        ASSERT_EQ(d.x_index(), (Ivec{3, 0, 4, 1, 2}));
        ASSERT_EQ(d.y_index(), (Ivec{3, 1, 0, 2, 4}));
    }

    return 0;
}
