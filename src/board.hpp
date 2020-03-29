#pragma once

#include "warband.hpp"

#include <array>
#include <ostream>

namespace hsbg {
	/// A board consisting of an allied warband (first) and an enemy warband (second).
	using board = std::array<warband, 2>;

	auto operator<<(std::ostream& out, board const& board) -> std::ostream&;

	//! Prints a representation of the board with text formatting.
	auto pretty_print(board const& board) -> void;
}
