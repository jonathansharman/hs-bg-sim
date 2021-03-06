#pragma once

#include "combat_results.hpp"

#include <fmt/color.h>
#include <fmt/format.h>

namespace hsbg {
	auto combat_results::win_rate() const -> double {
		return static_cast<double>(wins) / n_trials;
	}

	auto combat_results::draw_rate() const -> double {
		return static_cast<double>(draws) / n_trials;
	}

	auto combat_results::loss_rate() const -> double {
		return static_cast<double>(losses) / n_trials;
	}

	auto combat_results::net_score() const -> int {
		return allied_score - enemy_score;
	}

	auto combat_results::avg_allied_score() const -> double {
		return static_cast<double>(allied_score) / n_trials;
	}

	auto combat_results::avg_ememy_score() const -> double {
		return static_cast<double>(enemy_score) / n_trials;
	}

	auto combat_results::avg_net_score() const -> double {
		return static_cast<double>(net_score()) / n_trials;
	}

	auto combat_results::operator+=(combat_results const& that) -> combat_results& {
		n_trials += that.n_trials;
		wins += that.wins;
		draws += that.draws;
		losses += that.losses;
		allied_score += that.allied_score;
		enemy_score += that.enemy_score;
		return *this;
	}

	auto operator+(combat_results const& left, combat_results const& right) -> combat_results {
		combat_results result = left;
		result += right;
		return result;
	}

	auto operator<<(std::ostream& out, combat_results const& results) -> std::ostream& {
		auto fmt_string =
			"Trials: {}\n"
			"Wins/losses/draws: {} ({:.2f}%) / {} ({:.2f}%) / {} ({:.2f}%)\n"
			"Avg. score: {}";
		out << fmt::format(fmt_string,
			results.n_trials,
			results.wins,
			100.0 * results.win_rate(),
			results.draws,
			100.0 * results.draw_rate(),
			results.losses,
			100.0 * results.loss_rate(),
			results.avg_net_score());
		return out;
	}

	auto pretty_print(combat_results const& results) -> void {
		fmt::print("Trials: {}\n", results.n_trials);
		fmt::print("Wins/losses/draws: ");
		fmt::print(fmt::fg(fmt::color::lime_green), "{} ({:.2f}%)", results.wins, 100.0 * results.win_rate());
		fmt::print(" / ");
		fmt::print(fmt::fg(fmt::color::golden_rod), "{} ({:.2f}%)", results.draws, 100.0 * results.draw_rate());
		fmt::print(" / ");
		fmt::print(fmt::fg(fmt::color::indian_red), "{} ({:.2f}%)\n", results.losses, 100.0 * results.loss_rate());
		fmt::print("Avg. score: ");
		double const avg_net_score = results.avg_net_score();
		fmt::color const avg_net_score_color = avg_net_score > 0
			? fmt::color::lime_green
			: avg_net_score == 0.0 ? fmt::color::golden_rod : fmt::color::indian_red;
		fmt::print(fmt::fg(avg_net_score_color), "{}\n", avg_net_score);
	}
}
