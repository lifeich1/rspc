#include "floyd.hh"
#include "dense_net.hh"
#include "network_v_trait.hh"
#include "common/assert.hh"

int main() {
    using TraitI = A::NetworkVTrait<int>;
    typedef A::DenseNet<9, TraitI> Nw;
    typedef A::Floyd<9, Nw, TraitI> Fl;
    {
        Nw nw;
        Fl fl{&nw};
        nw.arrow(0, 2, 10);
        nw.arrow(2, 1, 10);
        nw.arrow(2, 3, 1);
        nw.arrow(1, 3, 10);
        fl(6);
        auto const & f = fl.f;
        ASSERT_EQ(f[0][1], 20);
        ASSERT_EQ(f[0][3], 11);
        ASSERT_EQ(f[0][2], 10);
        ASSERT_EQ(f[1][3], 10);
        ASSERT_EQ(fl.reach(3, 0), false);
        ASSERT_EQ(fl.reach(0, 3), true);
    }
    return 0;
}
