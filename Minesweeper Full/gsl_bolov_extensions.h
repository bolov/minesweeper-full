#pragma once

#pragma once

#include <cstddef>
#include <cstdint>

#include <utility>

#include "gsl/gsl"

namespace bolov {
namespace gslx {

// --- size

using size_t = std::ptrdiff_t;

constexpr auto size_cast(std::size_t size) noexcept -> gslx::size_t
{
    return static_cast<gslx::size_t>(size);
}

template <class T>
auto size_of()
{
    return size_cast(sizeof(T));
};

template <class T>
auto align_of()
{
    return size_cast(alignof(T));
};
}
}