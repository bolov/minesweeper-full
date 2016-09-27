#include <array>
#include <iostream>
#include <stdexcept>

#include "gsl\gsl"
#include "gsl_bolov_extensions.h"

#include "matrix.h"
#include "utils.h"
#include "string_algo.h"

#include "game.h"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

int main() try {
    cout << "Hello and wake up!" << endl << endl;

    using minesweeper::Game;

    Game game{8, 20, 25};
    game.main_loop();

    cout << "Bye Bye now" << endl;
}
catch (const gsl::fail_fast& e)
{
    cerr << "gsl contract violation: " << e.what() << endl;
}
catch (const std::exception& e)
{
    cerr << "std::exception : " << e.what() << endl;
}
