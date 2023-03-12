#pragma once

#ifndef NETWORK_V_TRAIT__UPD_LESS
#define NETWORK_V_TRAIT__UPD_LESS 1
#endif

namespace A {
template <class T, std::size_t DEF_V = 0, std::size_t SET_V = 1>
struct NetworkVTrait {
    typedef T edge_type;
    typedef T edge_value_t;

    inline static edge_value_t const & 
    edge_value(edge_type const * e) { return *e; }

    inline static bool edge_update(edge_type const & a, edge_type const & b) {
#if NETWORK_V_TRAIT__UPD_LESS
        return b < a;
#else
        return b > a;
#endif
    }

    static const edge_type EDGE_DEFAULT;
    static const edge_type EDGE_SET_VALUE;
};

template <class T, std::size_t DEF_V, std::size_t SET_V>
const typename NetworkVTrait<T, DEF_V, SET_V>::edge_type
NetworkVTrait<T, DEF_V, SET_V>::EDGE_DEFAULT = T(DEF_V);

template <class T, std::size_t DEF_V, std::size_t SET_V>
const typename NetworkVTrait<T, DEF_V, SET_V>::edge_type
NetworkVTrait<T, DEF_V, SET_V>::EDGE_SET_VALUE = T(SET_V);
}
