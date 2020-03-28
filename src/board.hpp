#pragma once

#include "ids.hpp"
#include "minion.hpp"

#include <array>
#include <optional>
#include <unordered_set>

//! Type representing a warband: a list of minions on one side of the board.
using warband = std::list<minion>;

using wb_it = warband::iterator;

struct board {
	std::array<warband, 2> wbs;

	//! Tracks which minion should attack next, if possible.
	std::array<wb_it, 2> current_attackers;

	//! Used for Kangor's Apprentice's deathrattle.
	std::array<std::vector<minion>, 2> dead_mechs_lists;

	//! Gets a reference to the warband to which the passed minion belongs.
	//! @note Behavior is undefined if the minion is not present in either warband.
	auto get_allies(wb_it target) -> warband&;

	//! Gets a reference to the warband opposing the input warband.
	auto get_enemies(warband& wb) -> warband&;

	//! Gets a reference to the list of dead friendly mechs for this warband.
	auto get_dead_mechs(warband& wb) -> std::vector<minion>&;

	//! Summons a minion to the given location, if there's room.
	//! @param summoned_by Reference to the warband that caused this summon.
	//! @param khadgars The set of Khadgars that have already participated in this summoning.
	auto summon( //
		warband& allies,
		wb_it target,
		minion summoned,
		warband const& summoned_by,
		std::unordered_set<minion*> khadgars) -> void;

	//! Kills the minion at the given location.
	auto trigger_dr(wb_it dying_it) -> void;

	//! Chooses a random attack target based on the given attacking warband and minion indices.
	auto choose_target(wb_it attacker) -> std::optional<wb_it>;

	auto take_damage(wb_it target, int damage, bool poisonous) -> void;

	//! Executes the given attack.
	auto attack(wb_it attacker, wb_it primary_target) -> void;

	auto resolve_deaths() -> void;

	//! Simulates combat and returns the resulting score.
	//! @param trace If true, intermediate board states will be printed.
	auto do_combat(bool trace = false) -> int;

	//! The sum of the first warband's tiers minus the sum of the second warband's tiers.
	auto score() const -> int;

	//! Prints a textual representation of the board to stdout.
	auto print() const -> void;
};
