#include "random.hpp"
#include "simulation.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

using namespace hsbg;

auto fuzz_test() -> void {
	for (int i = 0; i < 10'000; ++i) {
		simulate(board{random_warband(), random_warband()}, 1);
	}
}

auto too_many_minions_test() -> void {
	board const too_many_minions{//
		warband{//
			create(id::rat_pack).with_stats(7, 2).with_taunt(),
			create(id::scavenging_hyena)},
		warband{//
			create(id::cave_hydra).with_stats(2, 2),
			create(id::plant).with_attack(0),
			create(id::plant).with_attack(0)}};
	simulate(too_many_minions, 1, true);
}

auto where_to_put_brann(int n_trials = 10'000'000) -> void {
	warband const allies_without_brann{//
		create(id::king_bagurgle).with_stats(12, 3),
		create(id::old_murk_eye, true).with_stats(28, 55).with_poisonous().with_ds(),
		create(id::rockpool_hunter, true).with_stats(19, 54).with_poisonous().with_ds(),
		create(id::rockpool_hunter, true).with_stats(22, 53).with_poisonous().with_ds(),
		create(id::murloc_warleader, true).with_stats(16, 53).with_poisonous().with_ds(),
		create(id::murloc_warleader).with_stats(15, 26).with_poisonous().with_ds()};
	warband const enemies{//
		create(id::foe_reaper_4000).with_stats(6, 9),
		create(id::cobalt_guardian).with_stats(45, 24).with_taunt().with_ds(),
		create(id::bronze_warden).with_stats(28, 19),
		create(id::ghastcoiler),
		create(id::cave_hydra).with_stats(46, 35),
		create(id::voidlord).with_stats(33, 24),
		create(id::drakonid_enforcer, true).with_stats(24, 22)};
	std::vector<board> boards;
	for (int i = 0; i < 7; ++i) {
		auto allies = allies_without_brann;
		allies.insert(std::next(allies.begin(), i), create(id::brann_bronzebeard));
		boards.push_back({allies, enemies});
	}
	constexpr bool determine_position = false;
	if constexpr (determine_position) {
		for (int i = 0; i < 7; ++i) {
			fmt::print("Brann in position {}:\n{}", i, simulate(boards[i], n_trials));
		}
	} else {
		auto& actual_board = boards.back();
		pretty_print(simulate(actual_board, 100));
		fmt::print("Sample combat:\n");
		simulate(actual_board, 1, true);
	}
}

auto main() -> int {
	fuzz_test();

	return 0;
}
