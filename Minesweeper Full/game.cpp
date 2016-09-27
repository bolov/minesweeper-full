#include <iostream>

#include "gsl\gsl"
#include "gsl_bolov_extensions.h"

#include "game.h"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

namespace minesweeper {

auto Game::main_loop() -> void
{
    bolov::gslx::size_t i, j;

    while (true) {
        cout << grid_ << endl << endl;

        cout << "> ";
        cin >> i >> j;
        grid_.display_[i][j] = Grid::Display::e_shown;
    }
}
}
