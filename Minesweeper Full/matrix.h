#pragma once

#include <vector>

#include "gsl\gsl"
#include "gsl_bolov_extensions.h"

namespace bolov {
namespace containers {

template <class T> struct Line_proxy {
  gsl::span<T> line_ = {};

  Line_proxy() = default;
  Line_proxy(T *ptr, gslx::size_t size) : line_{ptr, size} {}

  auto operator*() const -> gsl::span<T> { return line_; }

  auto operator++() -> Line_proxy & {
    line_ = {line_.data() + line_.size(), line_.size()};
    return *this;
  }
  auto operator+(gslx::size_t inc) const -> Line_proxy {
    return {line_.data() + line_.size() * inc, line_.size()};
  }

  auto begin() const { return line_.begin(); }
  auto end() const { return line_.end(); }

  auto operator==(Line_proxy other) const -> bool {
    return line_ == other.line_;
  }
  auto operator!=(Line_proxy other) const -> bool { return !(*this == other); }
};

template <class T> class Matrix {
  std::vector<T> flat_data_ = {};
  gslx::size_t column_size_ = 0;
  gslx::size_t line_size_ = 0;

public:
  Matrix() = default;
  Matrix(gslx::size_t column_size, gslx::size_t line_size)
      : flat_data_(column_size * line_size), column_size_{column_size},
        line_size_{line_size} {}
  Matrix(gslx::size_t column_size, gslx::size_t line_size, const T &val)
      : flat_data_(column_size * line_size, val), column_size_{column_size},
        line_size_{line_size} {}

  auto operator[](gslx::size_t i) const -> gsl::span<const T> {
    return *(begin() + i);
  }
  auto operator[](gslx::size_t i) -> gsl::span<T> { return *(begin() + i); }

  auto flat_container() const -> const std::vector<T> & { return flat_data_; }
  auto flat_container() -> std::vector<T> & { return flat_data_; }

  auto flat_data() const -> const T * { return flat_data_.data(); }
  auto flat_data() -> T * { return flat_data_.data(); }

  auto begin() const -> Line_proxy<const T> {
    return {flat_data_.data(), line_size_};
  }
  auto begin() -> Line_proxy<T> { return {flat_data_.data(), line_size_}; }
  auto end() const -> Line_proxy<const T> { return begin() + column_size_; }
  auto end() -> Line_proxy<T> { return begin() + column_size_; }

  auto flat_begin() const { return flat_data_.begin(); }
  auto flat_begin() { return flat_data_.begin(); }
  auto flat_end() const { return flat_data_.end(); }
  auto flat_end() { return flat_data_.end(); }

  auto column_size() const { return column_size_; }
  auto line_size() const { return line_size_; }
  auto empty() const -> bool { return flat_data_.empty(); }
};
}
}