#pragma once

#include <unordered_map>
#include <string>

#include "gsl\gsl"
#include "gsl_bolov_extensions.h"

#include "grid.h"

namespace minesweeper {
using size_t = bolov::gslx::size_t;

class Cmd {
public:
    enum Type { e_show, e_bomb, e_flag, e_question };

private:
    static const std::unordered_map<Type, std::string> sk_spelling_;

    static auto get_type(const std::string& spelling) -> Type;

private:
    Type type_;

public:
    Cmd() = delete;
    Cmd(Type type) : type_{type} {}
    Cmd(const std::string& spelling) : Cmd{get_type(spelling)} {}

    auto spelling() const -> const std::string& { return sk_spelling_.at(type_); }
    auto type() const -> Type { return type_; }

    explicit operator std::string() const { return spelling(); }
    operator Type() const { return type(); }
};

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