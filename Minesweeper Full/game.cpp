#include <array>
#include <iostream>

#include "gsl\gsl"
#include "gsl_bolov_extensions.h"

#include "matrix.h"
#include "utils.h"

#include "grid.h"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

int main() try {

    cout << "It is alive" << endl << endl;

    using bolov::minesweeper::Grid;

    Grid grid{8, 20, 25};
    bolov::gslx::size_t i, j;



    while (true) {
        cout << grid << endl << endl;

        cout << "> ";
        cin >> i >> j;
        grid.hidden_[i][j] = false;
    }

    cout << "Ok go" << endl;
}
catch (gsl::fail_fast e)
{
    cerr << "gsl contract violation: " << e.what() << endl;
}