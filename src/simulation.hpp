#pragma once

#include "board.hpp"

#include <functional>
#include <optional>
#include <unordered_set>

namespace hsbg {
	using board = std::array<warband, 2>;

	/// Prints a textual representation of @p board to stdout.
	auto print(board const&) -> void;

	struct simulation {
		simulation(board const& board);

		/// Simulates combat and returns the resulting score.
		/// @param trace If true, intermediate board states will be printed.
		auto run(bool trace = false) -> int;

		/// Simulates combat multiple times and prints a summary of the results.
		/// @param trace If true, intermediate board states will be printed.
		auto simulate(int n_trials, bool trace = false) -> void;

		/// The sum of the first warband's tiers minus the sum of the second warband's tiers.
		auto score() const -> int;

	private:
		/// Current board state.
		board _board;

		/// Tracks which minion should attack next, if possible.
		std::array<wb_it, 2> _current_attackers;

		/// Used for Kangor's Apprentice's deathrattle.
		std::array<std::vector<minion>, 2> _dead_mechs_lists;

		/// Gets a reference to the warband to which the passed minion belongs.
		/// @note Behavior is undefined if the minion is not present in either warband.
		auto get_allies(wb_it target) -> warband&;

		/// Gets a reference to the warband opposing the input warband.
		auto get_enemies(warband& wb) -> warband&;

		/// Gets a reference to the list of dead friendly mechs for this warband.
		auto get_dead_mechs(warband& wb) -> std::vector<minion>&;

		/// Summons @p summoned to @p target in @p allies, as space permits.
		/// @param summoned_by Reference to the warband that caused this summon.
		/// @param khadgars The set of Khadgars that have already participated in this summoning.
		/// @return An iterator one past @p target, if summoning was successful, or @p target otherwise.
		auto summon( //
			warband& allies,
			wb_it target,
			minion summoned,
			warband const& summoned_by,
			std::unordered_set<minion*> khadgars) -> wb_it;

		/// Invokes @p generator @p count times and summons them to @p target, as space permits.
		auto summon_n( //
			int count,
			warband& allies,
			wb_it target,
			std::function<minion()> const& generator,
			warband const& summoned_by) -> void;

		/// Summons @p count copies of @p summoned at @p target, as space permits.
		auto summon_n( //
			int count,
			warband& allies,
			wb_it target,
			minion const& summoned,
			warband const& summoned_by) -> void;

		/// Kills the minion at the given location.
		auto trigger_dr(wb_it dying_it) -> void;

		/// Chooses a random attack target based on the given attacking warband and minion indices.
		auto choose_target(wb_it attacker) -> std::optional<wb_it>;

		/// Causes @p target to take @p damage optionally @p poisonous damage.
		auto take_damage(wb_it target, int damage, bool poisonous) -> void;

		/// Executes the given attack.
		auto attack(wb_it attacker, wb_it primary_target) -> void;

		/// Handles deathrattles, reborn, etc.
		auto resolve_deaths() -> void;
	};
}