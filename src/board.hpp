#pragma once

#include "minion.hpp"
#include "names.hpp"
#include "tiers.hpp"
#include "tribes.hpp"

#include <numeric>
#include <vector>

struct board {
	std::vector<minion> warband1;
	std::vector<minion> warband2;

	// Simulates combat and returns the amount of damage done as a result (the value is negative if warband2 wins).
	auto do_combat() -> int {
		// Do start-of-combat effects.
		// Perform attacks until one side is dead.
		while (!warband1.empty() && !warband2.empty()) {
			// POW!
			warband1.pop_back();
		}
		// Calculate score.
		return score();
	}

	// The sum of warband1's tiers minus the sum of warband2's tiers. Equal to damage dealt by minions, if combat has
	// occurred.
	auto score() const -> int {
		int const warband1_score = std::reduce(warband1.begin(), warband1.end(), 0, [](int sum, minion const& m) {
			return sum + static_cast<int>(get_tier(m.name));
		});
		int const warband2_score = std::reduce(warband2.begin(), warband2.end(), 0, [](int sum, minion const& m) {
			return sum - static_cast<int>(get_tier(m.name));
		});
		return warband1_score - warband2_score;
	}
};
