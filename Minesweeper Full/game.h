#pragma once

#include "gsl\gsl"
#include "gsl_bolov_extensions.h"

#include "grid.h"

namespace minesweeper {
using size_t = bolov::gslx::size_t;

class Game {
    Grid grid_;

public:
    Game() = delete;
    Game(size_t column_size, size_t line_size, size_t num_bombs)
        : grid_{column_size, line_size, num_bombs}
    {
    }

    auto main_loop() -> void;
};

} // ns minesweeper