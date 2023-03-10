#pragma once
#include <tuple>
#include <type_traits>
#include <functional>
#include <utility>

#include <iostream>

namespace A {

struct _vary_tuple_binop__plus_t {};
struct _vary_tuple_binop__minus_t {};
struct _vary_tuple_binop__mulitplies_t {};

template <class OP, typename... Ts, std::size_t... Is,
         std::enable_if_t<
             std::is_same_v<_vary_tuple_binop__plus_t, OP>, int> = 0>
std::tuple<Ts...> __binary_op_vary_tuple(OP op,
        std::tuple<Ts...> const & t1, std::tuple<Ts...> const & t2,
        std::index_sequence<Is...> const &)
{
    return { ((std::get<Is>(t1) + std::get<Is>(t2)))... };
}

template <class OP, typename... Ts, std::size_t... Is,
         std::enable_if_t<
             std::is_same_v<_vary_tuple_binop__minus_t, OP>, int> = 0>
std::tuple<Ts...> __binary_op_vary_tuple(OP op,
        std::tuple<Ts...> const & t1, std::tuple<Ts...> const & t2,
        std::index_sequence<Is...> const &)
{
    return { ((std::get<Is>(t1) - std::get<Is>(t2)))... };
}

template <class OP, typename... Ts, std::size_t... Is,
         std::enable_if_t<
             std::is_same_v<_vary_tuple_binop__mulitplies_t, OP>, int> = 0>
std::tuple<Ts...> __binary_op_vary_tuple(OP op,
        std::tuple<Ts...> const & t1, std::tuple<Ts...> const & t2,
        std::index_sequence<Is...> const &)
{
    return { ((std::get<Is>(t1) * std::get<Is>(t2)))... };
}

template <class OP, typename... Ts>
std::tuple<Ts...> _binop_vary_tuple(OP op,
        std::tuple<Ts...> const & t1,
        std::tuple<Ts...> const & t2
        )
{
    return  __binary_op_vary_tuple(op, t1, t2, std::index_sequence_for<Ts...>{});
}

template <typename... Ts>
struct VaryTuple {
    using type = VaryTuple<Ts...>;
    using tuple_t = std::tuple<Ts...>;

    std::tuple<Ts...> t;

    VaryTuple(std::tuple<Ts...> const & t): t{t} //{}
    { std::cout << "copy cons\n"; }
    VaryTuple(std::tuple<Ts...> && t): t{std::move(t)} //{}
    { std::cout << "move cons\n"; }

    type operator+ (type const & a) const { return _binop_vary_tuple(_vary_tuple_binop__plus_t{}, this->t, a.t); }
    type operator- (type const & a) const { return _binop_vary_tuple(_vary_tuple_binop__minus_t{}, this->t, a.t); }
    type operator* (type const & a) const { return _binop_vary_tuple(_vary_tuple_binop__mulitplies_t{}, this->t, a.t); }

    type & operator+= (type const & a) { return (*this = *this + a); }
    type & operator-= (type const & a) { return (*this = *this - a); }
    type & operator*= (type const & a) { return (*this = *this * a); }
};


template <typename... Ts>
auto vary_tuple(std::tuple<Ts...> && t) -> VaryTuple<Ts...> { return { t }; }
template <typename... Ts>
auto vary_tuple(std::tuple<Ts...> const & t) -> VaryTuple<Ts...> { return { t }; }
}
