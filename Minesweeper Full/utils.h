#pragma once

#include <algorithm>
#include "gsl\gsl"

namespace bolov {
namespace utils {


namespace detail {
template <class T>
struct Numeric_range_iterator {
    T val_;


    auto operator*() const -> T { return val_; }

    auto operator++() -> Numeric_range_iterator& {
        ++val_;
        return *this;
    }
    auto operator==(Numeric_range_iterator other) -> bool { return val_ == other.val_; }
    auto operator!=(Numeric_range_iterator other) -> bool { return !(*this == other); }
};
}

template <class T>
struct Numeric_range {
    using Iterator = detail::Numeric_range_iterator<T>;

    T begin_;
    T end_;

    Numeric_range() = delete;
    Numeric_range(T begin, T end) : begin_{begin}, end_{end} {
        Expects(begin <= end);
    }

    auto begin() const -> Iterator { return {begin_}; }
    auto end() const -> Iterator { return {end_}; }

    auto clamp_to(Numeric_range to) const -> Numeric_range {
        return {std::max(begin_, to.begin_), std::min(end_, to.end_)};
    }
};

using Size_range = Numeric_range<gslx::size_t>;

template <class T>
auto make_numeric_range(T begin, T end) -> Numeric_range<T>
{
    return {begin, end};
}

template <class Container>
auto make_size_range(const Container& cont) -> Size_range
{
    return {0, cont.size()};
}

template <class It, class T>
auto contains(It begin, It end, T value) -> bool
{
    return std::find(begin, end, value) != end;
}

template <class T>
auto contains(std::initializer_list<T> il, T value) -> bool
{
    return contains(std::begin(il), std::end(il), value);
}

} // ns utils
} // ns bolov
