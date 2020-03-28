#include "random.hpp"
#include "simulation.hpp"

#include <fmt/format.h>

using namespace hsbg;

auto fuzz_test() -> void {
	for (int i = 0; i < 10'000; ++i) {
		simulation{{random_warband(), random_warband()}}.run();
	}
}

auto where_to_put_brann() -> void {
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
	constexpr bool determine_position = true;
	if constexpr (determine_position) {
		for (int i = 0; i < 7; ++i) {
			fmt::print("Brann in position {}:\n", i);
			simulation{boards[i]}.simulate(10'000'000);
		}
	} else {
		auto& actual_board = boards.back();
		simulation{actual_board}.run(true);
		fmt::print("\n");
		simulation{actual_board}.simulate(1'000);
	}
}

auto main() -> int {
	fuzz_test();

	simulation{//
		board{//
			warband{//
				create(id::lightfang_enforcer),
				create(id::cave_hydra).with_stats(54, 41),
				create(id::murloc_tidecaller).with_stats(67, 61).with_ds(),
				create(id::primalfin_lookout).with_stats(30, 36).with_poisonous(),
				create(id::murloc_warleader, true).with_stats(34, 38).with_ds().with_poisonous(),
				create(id::brann_bronzebeard),
				create(id::cobalt_scalebane, true).with_stats(57, 59)},
			warband{//
				create(id::spawn_of_nzoth, true).with_stats(5, 5),
				create(id::mechano_egg, true).with_stats(7, 15).with_taunt().with_ds(),
				create(id::ghastcoiler).with_stats(9, 9),
				create(id::ghastcoiler),
				create(id::kangors_apprentice),
				create(id::kangors_apprentice, true),
				create(id::baron_rivendare)}}}
		.simulate(10'000);
}
