#pragma once

namespace A {
struct Network01Trait {
    typedef int edge_type;

    static const edge_type EDGE_DEFAULT;
    static const edge_type EDGE_SET_VALUE;
};
const Network01Trait::edge_type Network01Trait::EDGE_DEFAULT = 0;
const Network01Trait::edge_type Network01Trait::EDGE_SET_VALUE = 1;
} // namespace A
