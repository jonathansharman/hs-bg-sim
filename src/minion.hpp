#pragma once

#include "ids.hpp"
#include "stats.hpp"
#include "tiers.hpp"
#include "tribes.hpp"

#include <ostream>
#include <unordered_map>
#include <vector>

namespace hsbg {
	enum class dr { microbots, golden_microbots, plants };

	struct minion {
		id id;
		stats stats;

		bool poisoned = false;
		bool golden = false;
		bool taunt = false;
		bool ds = false;
		bool reborn = false;
		bool poisonous = false;
		int attack_count = 1;

		std::vector<dr> drs;

		minion(hsbg::id id, hsbg::stats stats);

		auto alive() const -> bool;
		auto dying() const -> bool;
		auto dead() const -> bool;

		auto make_alive() -> void;
		auto make_dying() -> void;
		auto make_dead() -> void;

		[[nodiscard]] auto with_stats(hsbg::stats stats) -> minion&;
		[[nodiscard]] auto with_attack(int attack) -> minion&;
		[[nodiscard]] auto with_health(int health) -> minion&;
		[[nodiscard]] auto with_taunt() -> minion&;
		[[nodiscard]] auto with_ds() -> minion&;
		[[nodiscard]] auto with_reborn(bool value = true) -> minion&;
		[[nodiscard]] auto with_poisonous() -> minion&;
		[[nodiscard]] auto with_windfury() -> minion&;
		[[nodiscard]] auto with_megawindfury() -> minion&;
		[[nodiscard]] auto with_dr(dr dr) -> minion&;

	private:
		enum class liveness { alive, dying, dead };

		liveness _liveness = liveness::alive;
	};

	auto operator<<(std::ostream& out, minion const& m) -> std::ostream&;

	/// Array of all base minions, ordered by ID.
	inline minion const all_minions[] = { //
		// Tier 1
		minion{id::alleycat, {1, 1}},
		minion{id::dire_wolf_alpha, {2, 2}},
		minion{id::dragonspawn_lieutenant, {2, 3}}.with_taunt(),
		minion{id::fiendish_servant, {2, 1}},
		minion{id::vulgar_homunculus, {2, 4}}.with_taunt(),
		minion{id::mecharoo, {1, 1}},
		minion{id::micro_machine, {1, 2}},
		minion{id::murloc_tidecaller, {1, 2}},
		minion{id::murloc_tidehunter, {2, 1}},
		minion{id::red_whelp, {1, 2}},
		minion{id::rockpool_hunter, {2, 3}},
		minion{id::righteous_protector, {1, 1}}.with_taunt().with_ds(),
		minion{id::selfless_hero, {2, 1}},
		minion{id::wrath_weaver, {1, 1}},
		// Tokens (tier 1)
		minion{id::big_bad_wolf, {3, 2}},
		minion{id::damaged_golem, {2, 1}},
		minion{id::finkle_einhorn, {3, 3}},
		minion{id::guard_bot, {2, 3}}.with_taunt(),
		minion{id::hyena, {2, 2}},
		minion{id::imp, {1, 1}},
		minion{id::ironhide_runt, {5, 5}},
		minion{id::jo_e_bot, {1, 1}},
		minion{id::microbot, {1, 1}},
		minion{id::murloc_scout, {1, 1}},
		minion{id::plant, {1, 1}},
		minion{id::rat, {1, 1}},
		minion{id::robosaur, {8, 8}},
		minion{id::spider, {1, 1}},
		minion{id::tabbycat, {1, 1}},
		minion{id::voidwalker, {1, 3}}.with_taunt(),
		minion{id::amalgam, {1, 1}},
		// Tier 2
		minion{id::glyph_guardian, {2, 4}},
		minion{id::harvest_golem, {2, 3}},
		minion{id::imprisoner, {3, 3}}.with_taunt(),
		minion{id::kaboom_bot, {2, 2}},
		minion{id::kindly_grandmother, {1, 1}},
		minion{id::metaltooth_leaper, {3, 3}},
		minion{id::murloc_warleader, {3, 3}},
		minion{id::nathrezim_overseer, {2, 3}},
		minion{id::old_murk_eye, {2, 4}},
		minion{id::pogo_hopper, {1, 1}},
		minion{id::rat_pack, {2, 2}},
		minion{id::scavenging_hyena, {2, 2}},
		minion{id::spawn_of_nzoth, {2, 2}},
		minion{id::steward_of_time, {3, 4}},
		minion{id::unstable_ghoul, {1, 3}}.with_taunt(),
		minion{id::waxrider_togwaggle, {1, 2}},
		minion{id::zoobot, {3, 3}},
		// Tier 3
		minion{id::bronze_warden, {2, 1}}.with_ds().with_reborn(),
		minion{id::cobalt_guardian, {6, 3}},
		minion{id::coldlight_seer, {2, 3}},
		minion{id::crowd_favorite, {4, 4}},
		minion{id::crystal_weaver, {5, 4}},
		minion{id::hangry_dragon, {4, 4}},
		minion{id::houndmaster, {4, 3}},
		minion{id::imp_gang_boss, {2, 4}},
		minion{id::infested_wolf, {3, 3}},
		minion{id::khadgar, {2, 2}},
		minion{id::pack_leader, {3, 3}},
		minion{id::piloted_shredder, {4, 3}},
		minion{id::replicating_menace, {3, 1}},
		minion{id::screwjank_clunker, {2, 5}},
		minion{id::shifter_zerus, {1, 1}},
		minion{id::soul_juggler, {3, 3}},
		minion{id::the_beast, {9, 7}},
		minion{id::twilight_emissary, {4, 4}}.with_taunt(),
		// Tier 4
		minion{id::annoy_o_module, {2, 4}}.with_ds().with_taunt(),
		minion{id::bolvar_fireblood, {1, 7}}.with_ds(),
		minion{id::cave_hydra, {2, 4}},
		minion{id::cobalt_scalebane, {5, 5}},
		minion{id::defender_of_argus, {2, 3}},
		minion{id::drakonid_enforcer, {3, 6}},
		minion{id::floating_watcher, {4, 4}},
		minion{id::herald_of_flame, {5, 6}},
		minion{id::iron_sensei, {2, 2}},
		minion{id::mechano_egg, {0, 5}},
		minion{id::menagerie_magician, {4, 4}},
		minion{id::savannah_highmane, {6, 5}},
		minion{id::security_rover, {2, 6}},
		minion{id::siegebreaker, {5, 8}}.with_taunt(),
		minion{id::toxfin, {1, 2}},
		minion{id::virmen_sensei, {4, 5}},
		// Tier 5
		minion{id::annihilan_battlemaster, {3, 1}},
		minion{id::baron_rivendare, {1, 7}},
		minion{id::brann_bronzebeard, {2, 4}},
		minion{id::goldrinn_the_great_wolf, {4, 4}},
		minion{id::ironhide_direhorn, {7, 7}},
		minion{id::junkbot, {1, 5}},
		minion{id::king_bagurgle, {6, 3}},
		minion{id::lightfang_enforcer, {2, 2}},
		minion{id::malganis, {9, 7}},
		minion{id::murozond, {5, 5}},
		minion{id::primalfin_lookout, {3, 2}},
		minion{id::razoregore_the_untamed, {2, 4}},
		minion{id::sneeds_old_shredder, {5, 7}},
		minion{id::strongshell_scavenger, {2, 3}},
		minion{id::voidlord, {3, 9}}.with_taunt(),
		// Tier 6
		minion{id::foe_reaper_4000, {6, 9}},
		minion{id::gentle_megasaur, {5, 4}},
		minion{id::ghastcoiler, {7, 7}},
		minion{id::holy_mackerel, {8, 4}},
		minion{id::imp_mama, {6, 8}},
		minion{id::kangors_apprentice, {3, 6}},
		minion{id::kalecgos_arcane_aspect, {4, 12}},
		minion{id::maexxna, {2, 8}}.with_poisonous(),
		minion{id::mama_bear, {5, 5}},
		minion{id::nadina_the_red, {7, 4}},
		minion{id::zapp_slywick, {7, 10}}.with_windfury()};

	/// Creates a fresh, optionally golden copy of the minion with the given @p id.
	auto create(id id, bool golden = false) -> minion;
}
