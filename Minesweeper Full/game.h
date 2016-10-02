#pragma once

#include <unordered_map>
#include <string>
#include <tuple>

#include "gsl\gsl"
#include "gsl_bolov_extensions.h"

#include "grid.h"

namespace minesweeper {
using size_t = bolov::gslx::size_t;

class Cmd {
public:
    enum Type { e_show, e_flag, e_question, e_clear, e_reset, e_quit, e_help };

private:
    static const std::unordered_map<Type, std::string> sk_spelling_;

private:
    Type type_;

public:
    Cmd() = delete;
    Cmd(Type type) : type_{type} {}
    Cmd(gsl::cstring_span<> spelling) : Cmd{bolov::utils::string_to_enum(spelling, sk_spelling_)} {}

    auto is_simple() const -> bool {
        return bolov::utils::contains({e_reset, e_quit, e_help}, type_);
    }

    auto spelling() const -> const std::string& { return sk_spelling_.at(type_); }
    auto type() const -> Type { return type_; }

    explicit operator std::string() const { return spelling(); }
    operator Type() const { return type(); }
};

class Full_cmd {
private:
    static const std::string usage_;
    static auto parse_line(gsl::cstring_span<> line)->std::tuple<Cmd, size_t, size_t>;
public:
    static auto usage() { return usage_; }

private:
    Cmd cmd_;
    size_t line_idx_;
    size_t column_idx_;

private:
    Full_cmd(std::tuple<Cmd, size_t, size_t> t)
        : cmd_{std::get<0>(t)}, line_idx_{std::get<1>(t)}, column_idx_{std::get<2>(t)}
    {
    }

public:
    Full_cmd(gsl::cstring_span<> spelling) : Full_cmd{parse_line(spelling)} {}

    auto is_simple() const -> bool { return cmd().is_simple(); }
    auto cmd() const -> Cmd { return cmd_; }
    auto line_idx() const -> size_t { Expects(!is_simple()); return line_idx_; }
    auto column_idx() const -> size_t { Expects(!is_simple()); return column_idx_; }
};

struct Game_params {
    size_t column_size;
    size_t line_size;
    size_t num_bombs;
};

class Game {
public:
    enum class Difficulty { e_easy, e_medium, e_difficult, e_custom };

    static const std::string welcome_;

private:
    static const std::unordered_map<Difficulty, std::string> sk_difficulty_spelling_;
    static const std::unordered_map<Difficulty, Game_params> sk_difficulty_params_;

public:
    static auto get_params() -> Game_params;
    static auto get_custom_params() -> Game_params;

private:
    Grid grid_;

public:
    Game() = delete;
    Game(size_t column_size, size_t line_size, size_t num_bombs)
        : grid_{column_size, line_size, num_bombs}
    {
    }
    Game(Game_params params) : Game{params.column_size, params.line_size, params.num_bombs} {}

    auto main_loop() -> void;
};

} // ns minesweeper