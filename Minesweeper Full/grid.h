#pragma once

#include <random>
#include <algorithm>

#include "gsl\gsl"
#include "gsl_bolov_extensions.h"

#include "utils.h"
#include "matrix.h"

namespace bolov {
namespace minesweeper {

class Grid {
public:
    static const int sk_bomb_cell = -1;

private:
public: // TODO delete
    containers::Matrix<int> bombs_;
    containers::Matrix<int> hidden_;
    gslx::size_t num_bombs_;

    auto get_num_neighbour_bombs(gslx::size_t i, gslx::size_t j) -> int
    {
        using utils::Size_range;
        using utils::make_size_range;

        int sum = 0;
        for (auto ni : Size_range{i - 1, i + 2}.clamp_to({0, bombs_.column_size()})) {
            for (auto nj : utils::Size_range{j - 1, j + 2}.clamp_to({0, bombs_.line_size()})) {
                if (ni == i && nj == j)
                    continue;
                if (bombs_[ni][nj] == sk_bomb_cell)
                    ++sum;
            }
        }
        return sum;
    }

public:
    Grid() = delete;
    Grid(gslx::size_t column_size, gslx::size_t line_size, gslx::size_t num_bombs)
        : bombs_(column_size, line_size), hidden_(column_size, line_size, true),
          num_bombs_{num_bombs}
    {
        Expects(column_size > 0);
        Expects(line_size > 0);
        //Expects(num_bombs > 0 && num_bombs <= column_size * line_size);

        std::mt19937 random_engine{std::random_device{}()};

        // put random bombs
        std::fill_n(bombs_.flat_begin(), num_bombs_, sk_bomb_cell);
        std::shuffle(bombs_.flat_begin(), bombs_.flat_end(), random_engine);

        bombs_[0][0] = sk_bomb_cell;

        
        using utils::Size_range;
        // compute neighbours
        for (auto i : Size_range{0, bombs_.column_size()}) {
            for (auto j : Size_range{0, bombs_.line_size()}) {
                if (bombs_[i][j] != sk_bomb_cell)
                    bombs_[i][j] = get_num_neighbour_bombs(i, j);
            }
        }
        

        Ensures(!bombs_.empty() && !hidden_.empty());
        Ensures(bombs_.column_size() == hidden_.column_size());
        Ensures(bombs_.line_size() == hidden_.line_size());
    }
};

auto operator<<(std::ostream& os, const Grid& grid) -> std::ostream&
{
    using utils::Size_range;
    os << "   ";
    for (auto i : Size_range{0, grid.bombs_.line_size()})
        os << (i % 10) << " ";
    os << std::endl;
    os << "   ";
    for (auto i : Size_range{0, grid.bombs_.line_size()})
        os << "- ";
    os << std::endl << std::endl;

    for (auto i : Size_range{0, grid.bombs_.column_size()}) {
        os << (i % 10) << "| ";
        for (auto j : Size_range{0, grid.bombs_.line_size()}) {
            if (grid.hidden_[i][j]) {
                os << '?' << " ";
                continue;
            }
            if (grid.bombs_[i][j] == Grid::sk_bomb_cell)
                os << '*' << " ";
            else
                os << grid.bombs_[i][j] << " ";
        }
        os << std::endl << std::endl;
    }
    return os;
}
}
}