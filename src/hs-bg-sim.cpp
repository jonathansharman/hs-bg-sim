#include "board.hpp"
#include "minion.hpp"
#include "names.hpp"
#include "tiers.hpp"
#include "tribes.hpp"

#include <fmt/format.h>

auto main() -> int {
	auto const minion_map = get_minion_map();

	auto const two_cost_names = {//
		name::dire_wolf_alpha,
		name::vulgar_homunculus,
		name::micro_machine,
		name::murloc_tidehunter,
		name::rockpool_hunter,
		name::dragonspawn_lieutenant,
		name::kindly_grandmother,
		name::scavenging_hyena,
		name::unstable_ghoul,
		name::khadgar};

	auto make_minion = [&minion_map](name name) { return minion_map.at(name); };

	board b;
	b.warband1 = {//
		make_minion(name::foe_reaper_4000).with_stats(6, 9),
		make_minion(name::cobalt_guardian).with_stats(45, 24).with_taunt().with_ds(),
		make_minion(name::bronze_warden).with_stats(28, 19),
		make_minion(name::ghastcoiler),
		make_minion(name::cave_hydra).with_stats(46, 35),
		make_minion(name::voidlord).with_stats(33, 24),
		make_minion(name::drakonid_enforcer).with_golden().with_stats(24, 22)};
	b.warband2 = {//
		make_minion(name::king_bagurgle).with_stats(12, 3),
		make_minion(name::old_murk_eye).with_golden().with_stats(28, 55).with_poison().with_ds(),
		make_minion(name::rockpool_hunter).with_golden().with_stats(19, 54).with_poison().with_ds(),
		make_minion(name::rockpool_hunter).with_golden().with_stats(22, 53).with_poison().with_ds(),
		make_minion(name::murloc_warleader).with_golden().with_stats(16, 53).with_poison().with_ds(),
		make_minion(name::murloc_warleader).with_stats(15, 26).with_poison().with_ds(),
		make_minion(name::brann_bronzebeard)};

	int wins = 0;
	int draws = 0;
	int losses = 0;
	int total_score = 0;
	constexpr int n_trials = 100;
	for (int i = 0; i < n_trials; ++i) {
		board b_copy = b;
		int const score = b.do_combat();
		if (score > 0) {
			++wins;
		} else if (score < 0) {
			++losses;
		} else {
			++draws;
		}
		total_score += score;
	}
	fmt::print("After {} simulations...\n", n_trials);
	fmt::print("  Wins-loss-draw: {}-{}-{}\n", wins, losses, draws);
	fmt::print("  Avg. score: {}", 1.0 * total_score / n_trials);
}
