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

using namespace std::string_literals;

int main() try {
    namespace ms = minesweeper;

    cout << ms::Game::welcome_ << endl << endl;

    ms::Game_params params = ms::Game::get_params();

    ms::Game game{params};

    while (true) {
        auto cmd = game.main_loop();

        if (cmd == ms::Cmd::e_quit) {
            break;
        }

        if (cmd == ms::Cmd::e_reset) {
            game = ms::Game{params};
            continue;
        }

        if (cmd == ms::Cmd::e_new) {
            params = ms::Game::get_params();
            game = ms::Game{params};
            continue;
        }

    }

    cout << "Ty for playing. Bye bye" << endl << endl;
}
catch (const gsl::fail_fast& e)
{
    cerr << "gsl contract violation: " << e.what() << endl;
}
catch (const std::exception& e)
{
    cerr << "std::exception : " << e.what() << endl;
}
