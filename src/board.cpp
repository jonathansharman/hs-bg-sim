#include "board.hpp"

#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace hsbg {
	auto operator<<(std::ostream& out, board const& board) -> std::ostream& {
		auto fmt_string =
			"{}\n"
			"- VS -\n"
			"{}";
		out << fmt::format(fmt_string, board[0], board[1]);
		return out;
	}

	auto pretty_print(board const& board) -> void {
		fmt::print(fmt::fg(fmt::color::lime_green), "{}\n", board[0]);
		fmt::print(fmt::fg(fmt::color::indian_red), "{}\n", board[1]);
	}
}
