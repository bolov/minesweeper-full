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

auto test0()
{
    std::string line;

    while (std::getline(cin, line)) {
        try {
            minesweeper::Full_cmd full_cmd = line;

            cout << full_cmd.cmd().spelling() << " " << full_cmd.line_idx() << " "
                 << full_cmd.column_idx() << endl;
        }
        catch (const std::exception& e) {
            cerr << "Invalid command '"s << line << "': " << endl;
            cerr << e.what() << endl;
            cerr << "type help or h for usage" << endl;
        }
    }
}

int main() try {
    cout << "Hello and wake up!" << endl << endl;

    test0();

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
