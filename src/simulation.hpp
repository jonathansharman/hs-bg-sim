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

	/// The desired type of combat result.
	enum class goal { max_wins, min_losses, max_net_wins, max_damage_given, min_damage_taken, max_net_damage };

	/// Attempts to find the highest EV ordering for the allied warband.
	auto optimize_ally_order(board const& actual_board, int n_trials_per_alternative, goal goal = goal::max_net_wins) -> board;
}
