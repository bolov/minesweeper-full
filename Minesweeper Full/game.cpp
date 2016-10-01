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

using namespace std::string_literals;

const std::unordered_map<Cmd::Type, std::string> Cmd::sk_spelling_ = {
    {Cmd::e_show, "show"},
    {Cmd::e_flag, "flag"},
    {Cmd::e_question, "question"},
    {Cmd::e_clear, "clear"},
    {Cmd::e_reset, "reset"},
    {Cmd::e_quit, "quit"},
    {Cmd::e_help, "help"},
};

auto Cmd::get_type(gsl::cstring_span<> spelling) -> Type
{
    if (spelling.empty())
        throw std::invalid_argument{"cannot convert '"s + gsl::to_string(spelling) + " to 'Cmd::Type"s};

    auto found_it = std::find_if(std::begin(sk_spelling_), std::end(sk_spelling_),
                                 [spelling = bolov::str::to_lower(spelling)](const auto& p) {
                                     return spelling == p.second ||
                                            (spelling.size() == 1 && spelling[0] == p.second[0]);
                                     return true;
                                 });

    if (found_it == std::end(sk_spelling_))
        throw std::invalid_argument{"cannot convert '"s + gsl::to_string(spelling) +
                                    "' to Cmd::Type"s};

    return found_it->first;
}

const std::string Full_cmd::usage_ =
R"(Command usage:

show x y
flag x y
question x y
clear x y
reset
quit
help

Commands are case insensitive. As a shortcut, use only the first letter (except for quit). E.g.:
    s 2 4
    q 1 5
    r
    h
)";

auto Full_cmd::parse_line(gsl::cstring_span<> line) -> std::tuple<Cmd, size_t, size_t>
{
    auto tokens = bolov::str::split(line, ::isspace);

    if (tokens.empty())
        throw std::invalid_argument{"empty string as command"s};

    Cmd cmd = tokens[0];

    if (cmd.is_simple()) {
        if (tokens.size() != 1)
            throw std::invalid_argument{cmd.spelling() + " does not take any argument. "s +
                                        std::to_string(tokens.size()) + " provided"};

        return std::tuple<Cmd, size_t, size_t>{cmd, -1, -1};
    }
    else {
        if (tokens.size() != 3)
            throw std::invalid_argument{cmd.spelling() + " takes exactly 2 arguments. "s +
                                        std::to_string(tokens.size()) + " provided"};

        return std::tuple<Cmd, size_t, size_t>{tokens[0], bolov::str::str_to_int(tokens[1]),
                                               bolov::str::str_to_int(tokens[2])};
    }
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
