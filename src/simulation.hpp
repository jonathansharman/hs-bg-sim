#pragma once

#include "board.hpp"
#include "combat_results.hpp"

#include <functional>
#include <optional>
#include <unordered_set>

namespace hsbg {
	/// Simulates combat @p n_trials times and aggregates the results.
	/// @param trace If true, intermediate board states will be printed.
	auto simulate(board const& board, int n_trials, bool trace = false) -> combat_results;
}
