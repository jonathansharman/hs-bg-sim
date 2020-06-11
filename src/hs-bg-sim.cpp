#include "random.hpp"
#include "simulation.hpp"

#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

using namespace hsbg;

auto fuzz_test() -> void {
	constexpr int n_boards = 10'000;
	fmt::print("Simulating {} boards.\n", n_boards);
	for (int i = 0; i < n_boards; ++i) {
		simulate(board{random_warband(), random_warband()}, 1);
	}
	fmt::print("Done.\n");
}

auto too_many_minions_test() -> void {
	board const too_many_minions{//
		warband{//
			create(id::rat_pack).with_stats({7, 2}).with_taunt(),
			create(id::scavenging_hyena)},
		warband{//
			create(id::cave_hydra).with_stats({2, 2}),
			create(id::plant).with_attack(0),
			create(id::plant).with_attack(0)}};
	simulate(too_many_minions, 1, true);
}

auto where_to_put_brann(int n_trials = 1'000'000) -> void {
	warband const allies_without_brann{//
		create(id::king_bagurgle).with_stats({12, 3}),
		create(id::old_murk_eye, true).with_stats({28, 55}).with_poisonous().with_ds(),
		create(id::rockpool_hunter, true).with_stats({19, 54}).with_poisonous().with_ds(),
		create(id::rockpool_hunter, true).with_stats({22, 53}).with_poisonous().with_ds(),
		create(id::murloc_warleader, true).with_stats({16, 53}).with_poisonous().with_ds(),
		create(id::murloc_warleader).with_stats({15, 26}).with_poisonous().with_ds()};
	warband const enemies{//
		create(id::foe_reaper_4000).with_stats({6, 9}),
		create(id::cobalt_guardian).with_stats({45, 24}).with_taunt().with_ds(),
		create(id::bronze_warden).with_stats({28, 19}),
		create(id::ghastcoiler),
		create(id::cave_hydra).with_stats({46, 35}),
		create(id::voidlord).with_stats({33, 24}),
		create(id::drakonid_enforcer, true).with_stats({24, 22})};
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
		pretty_print(simulate(actual_board, n_trials));
		fmt::print("Sample combat:\n");
		simulate(actual_board, 1, true);
	}
}

auto murlocs_vs_dragons(int n_trials = 1'000'000) -> void {
	board const board{//
		warband{//
			create(id::king_bagurgle).with_stats({14, 3}),
			create(id::king_bagurgle).with_stats({18, 7}),
			create(id::king_bagurgle, true).with_stats({32, 50}).with_poisonous(),
			create(id::murloc_warleader, true).with_stats({40, 82}).with_poisonous(),
			create(id::coldlight_seer, true).with_stats({40, 60}).with_poisonous(),
			create(id::murloc_warleader, true).with_stats({32, 70}).with_poisonous(),
			create(id::brann_bronzebeard)},
		warband{//
			create(id::nadina_the_red),
			create(id::twilight_emissary, true).with_stats({39, 41}),
			create(id::razorgore_the_untamed).with_stats({79, 81}),
			create(id::cobalt_guardian, true).with_stats({28, 32}),
			create(id::kalecgos_arcane_aspect).with_stats({25, 33}),
			create(id::zoobot).with_stats({4, 5}),
			create(id::drakonid_enforcer, true).with_stats({28, 31})}};
	fmt::print("Sample combat:\n");
	simulate(board, 1, true);
	pretty_print(simulate(board, n_trials));

	fmt::print("Optimizing allied warband order for max net wins...\n");
	auto const best_board = optimize_ally_order(board, 1'000, goal::max_net_wins);
	if (best_board == board) {
		fmt::print("Order probably can't be improved much.");
	} else {
		fmt::print("Original allied warband...\n");
		fmt::print(fmt::fg(fmt::color::indian_red), "{}\n", board[0]);
		fmt::print("...could be improved to...\n");
		fmt::print(fmt::fg(fmt::color::lime_green), "{}\n", best_board[0]);
	}
}

auto demons_vs_exodia(int n_trials = 100'000) {
	board const board{//
		warband{//
			create(id::imprisoner).with_stats({9, 10}),
			create(id::fiendish_servant, true).with_stats({21, 22}),
			create(id::voidlord).with_stats({11, 17}),
			create(id::malganis).with_stats({12, 10}),
			create(id::malganis).with_stats({12, 10}),
			create(id::soul_juggler, true),
			create(id::soul_juggler).with_stats({4, 4})},
		warband{//
			create(id::unstable_ghoul, true),
			create(id::kaboom_bot),
			create(id::kaboom_bot).with_attack(4),
			create(id::rat_pack, true),
			create(id::spawn_of_nzoth, true),
			create(id::goldrinn_the_great_wolf),
			create(id::baron_rivendare)}};
	fmt::print("Sample combat:\n");
	simulate(board, 1, true);
	pretty_print(simulate(board, n_trials));
}

auto big_pogo(int n_trials = 1'000'000) {
	auto make_board = [](int health) {
		return board{//
			warband{//
				create(id::pogo_hopper, true).with_stats({166, health})},
			warband{//
				create(id::nadina_the_red),
				create(id::cobalt_scalebane).with_stats({7, 7}),
				create(id::herald_of_flame).with_stats({14, 12}),
				create(id::drakonid_enforcer),
				create(id::hangry_dragon, true).with_stats({29, 20}),
				create(id::razorgore_the_untamed).with_stats({46, 42}),
				create(id::waxrider_togwaggle).with_stats({5, 4})}};
	};
	for (int health = 166; health <= 227; ++health) {
		auto const results = simulate(make_board(health), n_trials);
		fmt::print("{}\t{}\t{}\t{}\n", health, results.win_rate(), results.draw_rate(), results.loss_rate());
	}
}

auto main() -> int {
	fuzz_test();

	return 0;
}
