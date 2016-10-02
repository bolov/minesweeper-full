#pragma once

#include <random>
#include <algorithm>

#include "gsl\gsl"
#include "gsl_bolov_extensions.h"

#include "utils.h"
#include "matrix.h"

namespace minesweeper {

using size_t = bolov::gslx::size_t;

class Grid {
public:
    using size_t = bolov::gslx::size_t;
    enum class Display {
        e_shown,
        e_hidden,
        e_flag,
        e_question,
    };
    enum class State {
        e_good,
        e_lose,
        e_win,
    };

    static const int sk_bomb_cell = -1;

private:
    bolov::containers::Matrix<int> bombs_;
    bolov::containers::Matrix<Display> display_;
    bolov::gslx::size_t num_bombs_;
    size_t num_flags_ = 0;
    State state_{State::e_good};

    auto around_size_range(size_t i, size_t j) const -> std::vector<std::pair<size_t, size_t>>
    {
        using bolov::utils::Size_range;

        std::vector<std::pair<size_t, size_t>> around;
        around.reserve(8);

        for (auto ni : Size_range{i - 1, i + 2}.clamp_to({0, column_size()})) {
            for (auto nj : Size_range{j - 1, j + 2}.clamp_to({0, line_size()})) {
                if (ni == i && nj == j)
                    continue;

                around.emplace_back(ni, nj);
            }
        }
        around.shrink_to_fit();
        return around;
    }

    auto get_num_neighbour_bombs(size_t i, size_t j) -> int
    {
        int sum = 0;
        for (auto idx : around_size_range(i, j)) {
            if (bombs_[idx] == sk_bomb_cell)
                ++sum;
        }
        return sum;
    }

    auto get_num_neighbour_flags(size_t i, size_t j) -> int
    {
        int sum = 0;
        for (auto idx : around_size_range(i, j)) {
            if (display_[idx] == Display::e_flag)
                ++sum;
        }
        return sum;
    }

public:
    Grid() = delete;
    Grid(size_t column_size, size_t line_size, size_t num_bombs)
        : bombs_(column_size, line_size), display_(column_size, line_size, Display::e_hidden),
          num_bombs_{num_bombs}
    {
        Expects(column_size > 0);
        Expects(line_size > 0);
        Expects(num_bombs > 0 && num_bombs <= column_size * line_size);

        std::mt19937 random_engine{std::random_device{}()};

        // put random bombs
        std::fill_n(bombs_.flat_begin(), num_bombs_, sk_bomb_cell);
        std::shuffle(bombs_.flat_begin(), bombs_.flat_end(), random_engine);

        using bolov::utils::Size_range;

        // compute neighbours
        for (auto i : Size_range{0, bombs_.column_size()}) {
            for (auto j : Size_range{0, bombs_.line_size()}) {
                if (bombs_[i][j] != sk_bomb_cell)
                    bombs_[i][j] = get_num_neighbour_bombs(i, j);
            }
        }

        Ensures(!bombs_.empty() && !display_.empty());
        Ensures(bombs_.column_size() == display_.column_size());
        Ensures(bombs_.line_size() == display_.line_size());
    }

    auto column_size() const -> size_t const { return bombs_.column_size(); }
    auto line_size() const -> size_t { return bombs_.line_size(); }

    auto are_idx_valid(size_t i, size_t j) const -> bool {
        return i >= 0 && i < column_size() && j >= 0 && j < line_size();
    }

    const auto& bombs() const { return bombs_; }
    const auto& display() const { return display_; }

    auto num_bombs() const { return num_bombs_; }
    auto num_flags() const { return num_flags_; }
    auto state() const { return state_; }

    auto check_state() -> void {
        if (state_ != State::e_good)
            return;

        size_t remaining = 0;

        for (auto d : display_.flat_container()) {
            if (d != Display::e_shown) {
                ++remaining;
            }
        }
        if (remaining == num_bombs_)
            state_ = State::e_win;
    }

    auto show(size_t i, size_t j) -> void {
        if (display_[i][j] == Display::e_flag)
            --num_flags_;

        if (bombs_[i][j] == 0) {
            expand(i, j);
            return;
        }

        display_[i][j] = Display::e_shown;

        if (bombs_[i][j] == sk_bomb_cell)
            state_ = State::e_lose;

    }

    auto flag(size_t i, size_t j) -> void
    {
        if (display_[i][j] == Display::e_shown)
            throw std::invalid_argument{"Cell is shown. Could not set flag."};

        if (display_[i][j] != Display::e_flag) {
            display_[i][j] = Display::e_flag;
            ++num_flags_;
        }
        else {
            display_[i][j] = Display::e_hidden;
            --num_flags_;
        }

    }

    auto question(size_t i, size_t j) -> void
    {
        if (display_[i][j] == Display::e_shown)
            throw std::invalid_argument{"Cell is shown. Could not set question."};

        if (display_[i][j] == Display::e_flag)
            --num_flags_;

        display_[i][j] = Grid::Display::e_question;
    }

    auto clear(size_t i, size_t j) -> void
    {
        if (display_[i][j] != Display::e_shown)
            throw std::invalid_argument{"Cell is not shown. Could not clear"};

        if (get_num_neighbour_flags(i, j) != bombs_[i][j])
            return;

        for (auto idx : around_size_range(i, j)) {
            if (display_[idx] != Display::e_shown && display_[idx] != Display::e_flag)
                show(idx.first, idx.second);
        }
    }

private:
    auto expand(size_t i, size_t j) -> void
    {
        if (display_[i][j] == Display::e_shown)
            return;

        display_[i][j] = Display::e_shown;

        if (bombs_[i][j] != 0)
            return;

        for (auto idx : around_size_range(i, j))
            expand(idx.first, idx.second);
    }
};

inline auto operator<<(std::ostream& os, const Grid& grid) -> std::ostream&
{
    using bolov::utils::Size_range;

    os << "   ";
    for (auto i : Size_range{0, grid.bombs().line_size()})
        os << (i % 10) << " ";
    os << std::endl;
    os << "   ";
    for (auto i : Size_range{0, grid.bombs().line_size()})
        os << "- ";
    os << std::endl << std::endl;

    for (auto i : Size_range{0, grid.bombs().column_size()}) {
        os << (i % 10) << "| ";
        for (auto j : Size_range{0, grid.bombs().line_size()}) {
            switch (grid.display()[i][j])
            {
            case Grid::Display::e_shown:
                if (grid.bombs()[i][j] == Grid::sk_bomb_cell)
                    os << 'X' << " ";
                else if (grid.bombs()[i][j] == 0)
                    os << ' ' << " ";
                else
                    os << grid.bombs()[i][j] << " ";
                break;
            case Grid::Display::e_hidden:
                os << '\"' << " ";
                break;
            case Grid::Display::e_flag:
                os << 'F' << " ";
                break;
            case Grid::Display::e_question:
                os << '?' << " ";
                break;
            }
        }
        os << std::endl << std::endl;
    }
    os << grid.num_flags() << " / " << grid.num_bombs() << std::endl;

    return os;
}
}