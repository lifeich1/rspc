#include "miller_rabin.hh"
#include "common/assert.hh"

typedef A::MillerRabinPrimeTest<> Ptest;

int main() {
    Ptest ptest;
    ASSERT_EQ(ptest(99999999), false);
    ASSERT_EQ(ptest(100000007), true);
    ASSERT_EQ(ptest(998244353), true);
    return 0;
}
