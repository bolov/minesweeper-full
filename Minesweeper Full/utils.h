#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
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

    auto begin() const -> Iterator { return{begin_}; }
    auto end() const -> Iterator { return{end_}; }

    auto clamp_to(Numeric_range to) const -> Numeric_range {
        return{std::max(begin_, to.begin_), std::min(end_, to.end_)};
    }
};

using Size_range = Numeric_range<gslx::size_t>;

template <class T>
auto make_numeric_range(T begin, T end) -> Numeric_range<T>
{
    return{begin, end};
}

template <class Container>
auto make_size_range(const Container& cont) -> Size_range
{
    return{0, cont.size()};
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

template <class E>
inline auto string_to_enum(gsl::cstring_span<> spelling,
    const std::unordered_map<E, std::string>& spelling_map,
    bool allow_firs_letter = true)
{
    using namespace std::string_literals;

    if (spelling.empty())
        throw std::invalid_argument{"cannot convert '"s + gsl::to_string(spelling) + " to enum"s};

    auto found_it = std::find_if(std::begin(spelling_map), std::end(spelling_map),
        [spelling = bolov::str::to_lower(spelling),
        allow_firs_letter](const auto& p) {
        return spelling == p.second ||
            (allow_firs_letter && spelling.size() == 1 &&
                spelling[0] == p.second[0]);
        return true;
    });

    if (found_it == std::end(spelling_map))
        throw std::invalid_argument{"cannot convert '"s + gsl::to_string(spelling) + "' to enum"s};

    return found_it->first;
}

inline auto& get_line(std::string& line, gsl::cstring_span<> prefix)
{
    std::cout << gsl::to_string(prefix);
    return std::getline(std::cin, line);
}

} // ns utils
} // ns bolov
