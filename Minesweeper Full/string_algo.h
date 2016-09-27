#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "gsl\gsl"
#include "gsl_bolov_extensions.h"

#include "utils.h"

namespace bolov {
namespace str {

template <class F>
inline auto split_impl(gsl::cstring_span<> str, F is_delim)
-> std::vector<gsl::cstring_span<>>
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
    return split_impl(str, [str, delims](char c) {
        return utils::contains(std::begin(delims), std::end(delims), c);
    });
}

} // ns str
} // ns bolov
