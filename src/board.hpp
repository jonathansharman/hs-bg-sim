#pragma once

#include "warband.hpp"

#include <array>

namespace hsbg {
	/// A board consisting of an allied warband and an enemy warband.
	using board = std::array<warband, 2>;

	/// Prints a textual representation of @p board to stdout.
	auto print(board const&) -> void;
}
