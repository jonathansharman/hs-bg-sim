#include "board.hpp"
#include "random.hpp"

#include <fmt/format.h>

auto simulate(board const& b, int n_trials, bool trace = false) {
	int wins = 0;
	int draws = 0;
	int losses = 0;
	int total_score = 0;
	// If not printing a full trace, at least print the starting board.
	if (!trace) { b.print(); }
	for (int i = 0; i < n_trials; ++i) {
		board b_copy = b;
		int const score = b_copy.do_combat(trace);
		if (score > 0) {
			++wins;
		} else if (score < 0) {
			++losses;
		} else {
			++draws;
		}
		total_score += score;
	}
	fmt::print("==============================\n");
	fmt::print("After {} simulation{}...\n", n_trials, n_trials == 1 ? "" : "s");
	fmt::print("  wins: {} ({}%), draws: {} ({}%), losses: {} ({}%)\n",
		wins,
		100.0 * wins / n_trials,
		draws,
		100.0 * draws / n_trials,
		losses,
		100.0 * losses / n_trials);
	fmt::print("  mean score: {}\n\n", 1.0 * total_score / n_trials);
}

auto random_warband() -> warband {
	warband result;
	int const count = rand_int(1, 7);
	for (int i = 0; i < count; ++i) {
		id const random_id = static_cast<id>(rand_int(0, static_cast<int>(id::COUNT) - 1));
		result.push_back(create(random_id));
	}
	return result;
};

auto fuzz_test() -> void {
	for (int i = 0; i < 10'000; ++i) {
		simulate(board{random_warband(), random_warband()}, 1);
	}
}

auto main() -> int {
	// fuzz_test();

	simulate(
		board{//
			warband{//
				create(id::voidlord).with_stats(1, 1),
				create(id::cave_hydra).with_stats(1, 1)},
			warband{//
				create(id::plant).with_stats(3, 1),
				create(id::plant).with_stats(3, 1),
				create(id::plant).with_stats(3, 1),
				create(id::plant).with_stats(3, 1),
				create(id::plant).with_stats(3, 1)}},
		1,
		true);

	board const without_brann{//
		warband{//
			create(id::king_bagurgle).with_stats(12, 3),
			create(id::old_murk_eye).make_golden().with_stats(28, 55).with_poisonous().with_ds(),
			create(id::rockpool_hunter).make_golden().with_stats(19, 54).with_poisonous().with_ds(),
			create(id::rockpool_hunter).make_golden().with_stats(22, 53).with_poisonous().with_ds(),
			create(id::murloc_warleader).make_golden().with_stats(16, 53).with_poisonous().with_ds(),
			create(id::murloc_warleader).with_stats(15, 26).with_poisonous().with_ds()},
		warband{//
			create(id::foe_reaper_4000).with_stats(6, 9),
			create(id::cobalt_guardian).with_stats(45, 24).with_taunt().with_ds(),
			create(id::bronze_warden).with_stats(28, 19),
			create(id::ghastcoiler),
			create(id::cave_hydra).with_stats(46, 35),
			create(id::voidlord).with_stats(33, 24),
			create(id::drakonid_enforcer).make_golden().with_stats(24, 22)}};
	std::vector<board> boards(7, without_brann);
	for (int i = 0; i < 7; ++i) {
		boards[i].wbs[0].insert(std::next(boards[i].wbs[0].begin(), i), create(id::brann_bronzebeard));
		// fmt::print("Brann in position {}:\n", i);
		// simulate(boards[i], 1'000'000);
	}

	// auto& actual_board = boards.back();
	// simulate(actual_board, 1, true);
	// simulate(actual_board, 1'000);
}
