#pragma once

#include <ostream>

namespace hsbg {
	/// Aggregate results from zero or more combats.
	struct combat_results {
		/// Number of trials.
		int n_trials = 0;
		/// The number of allied wins.
		int wins = 0;
		/// The number of draws.
		int draws = 0;
		/// The number of enemy wins.
		int losses = 0;
		/// The sum of allied minion tiers.
		int allied_score = 0;
		/// The sum of enemy minion tiers.
		int enemy_score = 0;

		/// Wins over number of trials.
		auto win_rate() const -> double;
		/// Draws over number of trials.
		auto draw_rate() const -> double;
		/// Losses over number of trials.
		auto loss_rate() const -> double;
		/// The difference between total allied score and total enemy score.
		auto net_score() const -> int;
		/// Allied score over number of trials.
		auto avg_allied_score() const -> double;
		/// Enemy score over number of trials.
		auto avg_ememy_score() const -> double;
		/// Net score over number of trials.
		auto avg_net_score() const -> double;

		auto operator+=(combat_results const& that) -> combat_results&;
	};

	auto operator+(combat_results const& left, combat_results const& right) -> combat_results;

	auto operator<<(std::ostream& out, combat_results const& results) -> std::ostream&;
}
