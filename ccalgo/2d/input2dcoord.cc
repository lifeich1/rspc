#include "input2dcoord.hh"

#include "common/assert.hh"

int main() {
    typedef A::Input2dCoord<int> I2C;
    typedef typename I2C::Pn Pn;
    typedef typename I2C::Vec Vec;

    Vec v;

    I2C(std::stringstream{"1 2\n3 4\n"}, 2) >> v;
    ASSERT_EQ(v, (Vec{Pn(1, 2), Pn(3, 4)}));

    I2C(std::stringstream{"11 2  3 44  99 77\n"}, 3) >> v;
    ASSERT_EQ(v, (Vec{Pn(11, 2), Pn(3, 44), Pn(99, 77)}));

    std::stringstream ss{"11 2  3 44  99 77\n"};
    I2C(ss, 3) >> v;
    ASSERT_EQ(v, (Vec{Pn(11, 2), Pn(3, 44), Pn(99, 77)}));

    return 0;
}
