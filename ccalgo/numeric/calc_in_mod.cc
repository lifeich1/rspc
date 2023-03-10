#include "calc_in_mod.hh"

#include "common/assert.hh"

using Ca = A::CalcInMod<998244353>;

int main() {
    Ca a = 992;
    Ca inv_a = a.mul_inv();
    ASSERT_EQ((a * inv_a).as_i(), 1);

    a = 3;
    a -= 1;
    ASSERT_EQ(a.v, 2);

    a *= 8;
    ASSERT_EQ(a.v, 16);
    a += 4;
    ASSERT_EQ(a.v, 20);

    ASSERT_EQ((a - 5).v, 15);
    ASSERT_EQ((a + 5).v, 25);
    ASSERT_EQ((a * 5).v, 100);

    return 0;
}
