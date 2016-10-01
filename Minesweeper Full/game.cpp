#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>

#include "gsl\gsl"
#include "gsl_bolov_extensions.h"

#include "string_algo.h"

#include "game.h"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

namespace minesweeper {

const std::unordered_map<Cmd::Type, std::string> Cmd::sk_spelling_ = {
    {Cmd::e_show, "show"},
    {Cmd::e_bomb, "bomb"},
    {Cmd::e_flag, "flag"},
    {Cmd::e_question, "question"},
};

auto Cmd::get_type(gsl::cstring_span<> spelling) -> Type
{
    using namespace std::string_literals;

    if (spelling.empty())
        throw std::invalid_argument{"cannot convert '"s + gsl::to_string(spelling) + "'Cmd::Type"s};

    auto found_it = std::find_if(std::begin(sk_spelling_), std::end(sk_spelling_),
                                 [spelling = bolov::str::to_lower(spelling)](const auto& p) {
                                     return spelling == p.second ||
                                            (spelling.size() == 1 && spelling[0] == p.second[0]);
                                     return true;
                                 });

    if (found_it == std::end(sk_spelling_))
        throw std::invalid_argument{"cannot convert '"s + gsl::to_string(spelling) + "'Cmd::Type"s};

    return found_it->first;
}

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
