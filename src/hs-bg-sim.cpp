#include "random.hpp"
#include "simulation.hpp"

#include <fmt/format.h>

using namespace hsbg;

auto fuzz_test() -> void {
	for (int i = 0; i < 10'000; ++i) {
		simulation{{random_warband(), random_warband()}}.run();
	}
}

auto main() -> int {
	fuzz_test();

	warband const allies_without_brann{//
		create(id::king_bagurgle).with_stats(12, 3),
		create(id::old_murk_eye).make_golden().with_stats(28, 55).with_poisonous().with_ds(),
		create(id::rockpool_hunter).make_golden().with_stats(19, 54).with_poisonous().with_ds(),
		create(id::rockpool_hunter).make_golden().with_stats(22, 53).with_poisonous().with_ds(),
		create(id::murloc_warleader).make_golden().with_stats(16, 53).with_poisonous().with_ds(),
		create(id::murloc_warleader).with_stats(15, 26).with_poisonous().with_ds()};
	warband const enemies{//
		create(id::foe_reaper_4000).with_stats(6, 9),
		create(id::cobalt_guardian).with_stats(45, 24).with_taunt().with_ds(),
		create(id::bronze_warden).with_stats(28, 19),
		create(id::ghastcoiler),
		create(id::cave_hydra).with_stats(46, 35),
		create(id::voidlord).with_stats(33, 24),
		create(id::drakonid_enforcer).make_golden().with_stats(24, 22)};
	std::vector<board> boards;
	for (int i = 0; i < 7; ++i) {
		auto allies = allies_without_brann;
		allies.insert(std::next(allies.begin(), i), create(id::brann_bronzebeard));
		boards.push_back({allies, enemies});
		fmt::print("Brann in position {}:\n", i);
		simulation{boards[i]}.simulate(10'000'000);
	}

	//auto& actual_board = boards.back();
	//simulation{actual_board}.run(true);
	//fmt::print("\n");
	//simulation{actual_board}.simulate(1'000);
}
