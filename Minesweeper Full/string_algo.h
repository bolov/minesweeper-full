#pragma once

#include <cstdlib>
#include <cctype> // to_lower

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>

#include "gsl\gsl"
#include "gsl_bolov_extensions.h"

#include "utils.h"

namespace bolov {
namespace str {

template <class F, class Enable = decltype(std::declval<bool&>() = std::declval<F>()('a'))>
inline auto split(gsl::cstring_span<> str, F is_delim) -> std::vector<gsl::cstring_span<>>
{
    std::vector<gsl::cstring_span<>> tokens;
    tokens.reserve(str.size() / 5); // average word length

    auto token_begin = decltype(str.begin()){};
    auto token_end = str.begin();

    while ((token_begin = std::find_if_not(token_end, str.end(), is_delim)) != str.end())
    {
        token_end = std::find_if(token_begin, str.end(), is_delim);
        tokens.emplace_back(str.subspan(std::distance(str.begin(), token_begin),
            std::distance(token_begin, token_end)));
    }
    return tokens;
}

inline auto split(gsl::cstring_span<> str, gsl::cstring_span<> delims)
    -> std::vector<gsl::cstring_span<>>
{
    return split(str, [str, delims](char c) {
        return utils::contains(std::begin(delims), std::end(delims), c);
    });
}

inline auto str_to_int(gsl::cstring_span<> str) -> int
{
    using namespace std::string_literals;

    char* last;
    errno = 0;

    auto value = std::strtoll(str.data(), &last, 10);

    if (last < str.data() + str.size())
        throw std::out_of_range{"string '"s + gsl::to_string(str) +
                                "' does not represent an integral value "s};

    try {
        return gsl::narrow<int>(value);
    }
    catch (gsl::narrowing_error&) {
        throw std::out_of_range{"value in string '"s + gsl::to_string(str) +
                                "' is out of range for `int`"};
    }
    catch (...) {
        throw;
    }
}

inline auto to_lower(gsl::cstring_span<> str) -> std::string
{
    std::string lower;
    lower.reserve(str.size());

    std::transform(std::begin(str), std::end(str), std::back_inserter(lower), ::tolower);
    return lower;
}

} // ns str
} // ns bolov
