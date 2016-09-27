#include <array>
#include <iostream>

#include "gsl\gsl"
#include "gsl_bolov_extensions.h"

#include "matrix.h"
#include "utils.h"

#include "game.h"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

int main() try {

    cout << "It is alive" << endl << endl;

    using minesweeper::Game;

    Game game{8, 20, 25};
    game.main_loop();

    cout << "Ok go" << endl;
}
catch (gsl::fail_fast e)
{
    cerr << "gsl contract violation: " << e.what() << endl;
}