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

const std::string Game::welcome_ = "Minesweeper. Enjoy!";

const std::unordered_map<Game::Difficulty, std::string> Game::sk_difficulty_spelling_ = {
    {Difficulty::e_easy, "easy"},
    {Difficulty::e_medium, "medium"},
    {Difficulty::e_difficult, "hard"},
    {Difficulty::e_custom, "custom"},
};

const std::unordered_map<Game::Difficulty, Game_params> Game::sk_difficulty_params_ = {
    {Difficulty::e_easy, {6, 4, 5}},
    {Difficulty::e_medium, {7, 10, 10}},
    {Difficulty::e_difficult, {8, 20, 25}},
    {Difficulty::e_difficult, {-1, -1, -1}},
};

auto Game::get_custom_params() -> Game_params
{
    std::string line;

    while (true) {
        cout << "Enter column-size line-size number-of-bombs" << endl;
        bolov::utils::get_line(line, "> ");

        auto tokens = bolov::str::split(line, ::isspace);

        if (tokens.size() != 3) {
            cerr << "3 arguments required" << endl << endl;
            continue;
        }

        try {
            Game_params params{bolov::str::str_to_int(tokens[0]), bolov::str::str_to_int(tokens[1]),
                               bolov::str::str_to_int(tokens[2])};

            if (params.column_size < 4 || params.line_size < 4) {
                cerr << "grid size must be at least 4 4" << endl << endl;
                continue;
            }

            if (params.num_bombs < 1) {
                cerr << "there must be at least 1 bomb" << endl << endl;
                continue;
            }

            if (params.num_bombs >= params.column_size * params.line_size) {
                cerr << "Too many bombs" << endl << endl;
                continue;
            }

            cout << endl;
            return params;
        }
        catch (std::exception& e) {
            cerr << "Please enter 3 numbers: " << endl;
            cerr << e.what() << endl << endl;
        }
    }
}

auto Game::get_params()->Game_params {
    std::string line;

    while (true) {
        cout << "Select difficulty: ";

        for (const auto& p : sk_difficulty_spelling_)
            cout << p.second << " ";
        cout << endl;

        bolov::utils::get_line(line, "> ");

        try {
            auto difficulty = bolov::utils::string_to_enum(line, sk_difficulty_spelling_, false);

            if (difficulty == Difficulty::e_custom) {
                cout << endl;
                return get_custom_params();
            }

            cout << endl;
            return sk_difficulty_params_.at(difficulty);
            
        }
        catch (std::exception&) {
            cout << "Invalid command: '" << line << "'" << endl << endl;
        }
    }
}

auto Game::main_loop() -> void
{
    std::string line;

    cout << "New game. For help enter help... Duh" << endl;

    while (bolov::utils::get_line(line, "> ")) {
        try {
            minesweeper::Full_cmd full_cmd = line;

            cout << full_cmd.cmd().spelling() << " " << full_cmd.line_idx() << " "
                 << full_cmd.column_idx() << endl;
        }
        catch (const std::exception& e) {
            cerr << "Invalid command '"s << line << "': " << endl;
            cerr << e.what() << endl;
            cerr << "type help or h for usage" << endl << endl;
        }
    }
}
}
