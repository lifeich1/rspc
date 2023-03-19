#include "vec_arr_net.hh"

#include "common/assert.hh"

#define POSTORDER_TRAVEL__NOEDGEEXT
#include "postorder_travel.hh"

#include <iterator>

std::vector<int> es[9];
auto nw = A::net(es);
A::PostorderTravel<9, decltype(nw)> pt{&nw};

int main() {
    es[0] = {1, 3, 5, 7};
    es[1] = {2, 4};
    es[2] = {6};
    std::vector<int> vs;
    pt(0, std::back_inserter(vs));
    std::copy(vs.begin(), vs.end(), std::ostream_iterator<int> (std::cout, " "));
    std::cout << std::endl;
    ASSERT_EQ(vs, std::vector<int>({7, 5, 3, 4, 6, 2, 1, 0 }));
    return 0;
}
