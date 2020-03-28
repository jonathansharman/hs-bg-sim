#pragma once

#include "minion.hpp"

#include <list>

namespace hsbg {
	//! Type representing a warband: a list of minions on one side of the board.
	using warband = std::list<minion>;

	using wb_it = warband::iterator;

	//! Prints a textual representation of @p warband to stdout.
	auto print(warband const& warband) -> void;

	//! Creates a warband of 1-7 randomly chosen minions with default stats.
	auto random_warband() -> warband;
}
