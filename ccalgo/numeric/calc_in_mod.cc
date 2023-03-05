#include "calc_in_mod.hh"

#include "common/assert.hh"

using Ca = A::CalcInMod<998244353>;

int main() {
    Ca a = 992;
    Ca inv_a = a.mul_inv();
    ASSERT_EQ((a * inv_a).as_i(), 1);

    return 0;
}
