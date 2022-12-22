#pragma once
#include <tuple>
#include <type_traits>
#include <functional>

namespace A {
template <class T>
using get_cv_iterator_t =
std::conditional_t<std::is_const_v<T>,
    typename T::const_iterator, typename T::iterator>;

template <std::size_t N, class Tuple, class FirstType, class... Types>
struct _tuple_deref_impl {
    typedef typename FirstType::reference FirstRef;
    std::tuple<FirstRef, typename Types::reference...>
        operator() (Tuple const & t) {
        return std::tuple_cat(
                std::make_tuple(std::ref(*std::get<std::tuple_size_v<Tuple> - N>(t))),
                _tuple_deref_impl<N - 1, Tuple, Types...>{}(t)
                );
    }
};
template <class Tuple, class Type>
struct _tuple_deref_impl<1, Tuple, Type> {
    std::tuple<typename Type::reference> operator() (Tuple const & t) {
        return {*std::get<std::tuple_size_v<Tuple> - 1>(t)};
    }
};
template <std::size_t N, class... Types>
using tuple_deref = _tuple_deref_impl<N, std::tuple<Types...>, Types...>;

template <std::size_t N, class Tuple>
struct tuple_inc {
    void operator() (Tuple & t) {
        ++std::get<N - 1>(t);
        tuple_inc<N - 1, Tuple>{}(t);
    }
};
template <class Tuple>
struct tuple_inc<1, Tuple> {
    void operator() (Tuple & t) {
        ++std::get<0>(t);
    }
};

template <std::size_t N, class Tuple>
struct tuple_any_eq {
    bool operator() (Tuple const & lhs, Tuple const & rhs) {
        return std::get<N - 1>(lhs) == std::get<N - 1>(rhs)
            ? true
            : tuple_any_eq<N - 1, Tuple>{}(lhs, rhs);
    }
};
template <class Tuple>
struct tuple_any_eq<1, Tuple> {
    bool operator() (Tuple const & lhs, Tuple const & rhs) {
        return std::get<0>(lhs) == std::get<0>(rhs);
    }
};

template <class... Iters>
class ZipSList {
public:
    ZipSList(std::tuple<Iters...> first, std::tuple<Iters...> last)
        : _first{first}, _last{last}
    {}

    template <class Container>
    ZipSList<Iters..., get_cv_iterator_t<Container>> operator+ (Container & container) {
        return {
            std::tuple_cat(_first, std::forward_as_tuple(container.begin())),
            std::tuple_cat(_last, std::forward_as_tuple(container.end())),
        };
    }

    template <class InputIt>
    ZipSList<Iters..., std::decay_t<InputIt>> c(InputIt first, InputIt last) {
        return {
            std::tuple_cat(_first, std::forward_as_tuple(first)),
            std::tuple_cat(_last, std::forward_as_tuple(last)),
        };
    }

    class ZIter {
        typedef std::tuple<Iters...> Tuple;
        static constexpr std::size_t tuple_siz = std::tuple_size_v<Tuple>;
    public:
        typedef std::tuple<typename Iters::value_type...> value_type;
        typedef std::tuple<typename Iters::reference...> reference;

        reference operator* () const { return tuple_deref<tuple_siz, Iters...>{}(_it); }

        ZIter & operator++ () { tuple_inc<tuple_siz, Tuple>{}(_it); return *this; }
        ZIter operator++ (int) { auto t = *this; tuple_inc<tuple_siz, Tuple>{}(_it); return t; }

        bool operator==(ZIter const & other) const { return tuple_any_eq<tuple_siz, Tuple>{}(_it, other._it); }
        bool operator!=(ZIter const & other) const { return !tuple_any_eq<tuple_siz, Tuple>{}(_it, other._it); }
    private:
        friend class ZipSList;

        ZIter(Tuple const & it): _it{it} {}

        Tuple _it;
    };

    template <class F, class FR = std::invoke_result_t<F, typename Iters::reference...>>
    auto wrap(F && f) -> std::function<FR(typename ZIter::reference const &)>
    {
        return [&f](typename ZIter::reference const & t) {
            return std::apply(f, t);
        };
    }

    ZIter begin() const { return {_first}; }
    ZIter end() const { return {_last}; }

private:
    std::tuple<Iters...> _first;
    std::tuple<Iters...> _last;
};
template <class Container>
ZipSList<get_cv_iterator_t<Container>> zip_slist(Container & container) {
    return {std::forward_as_tuple(container.begin()),
        std::forward_as_tuple(container.end())};
}

template <class InputIt>
ZipSList<std::decay_t<InputIt>> zip_slist(InputIt first, InputIt last) {
    return {std::forward_as_tuple(first),
        std::forward_as_tuple(last)};
}
} // namespace A
