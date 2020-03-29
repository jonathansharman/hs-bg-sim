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
	enum class on_attack { herald_of_flame, ironhide_direhorn };
	enum class on_any_death { old_murk_eye };
	enum class on_friendly_death { scavenging_hyena, baron_rivendare, junkbot };
	enum class on_friendly_kill { waxrider_togwaggle };
	enum class on_damage { imp_gang_boss, security_rover, imp_mama };

	enum class liveness { alive, dying, dead };

	struct minion {
		id id;
		stats stats;

		liveness liveness = liveness::alive;
		bool poisoned = false;
		bool golden = false;
		bool taunt = false;
		bool ds = false;
		bool reborn = false;
		bool poisonous = false;
		int attack_count = 1;

		std::vector<dr> drs;
		std::vector<on_attack> on_attacks;
		std::vector<on_friendly_death> on_friendly_deaths;
		std::vector<on_friendly_kill> on_friendly_kills;
		std::vector<on_damage> on_damages;

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
		[[nodiscard]] auto with_on_attack(on_attack on_attack) -> minion&;
		[[nodiscard]] auto with_on_friendly_death(on_friendly_death on_friendly_death) -> minion&;
		[[nodiscard]] auto with_on_friendly_kill(on_friendly_kill on_friendly_kill) -> minion&;
		[[nodiscard]] auto with_on_damage(on_damage on_damage) -> minion&;
	};

	auto operator<<(std::ostream& out, minion const& m) -> std::ostream&;

	/// Array of all base minions, ordered by ID.
	inline minion const all_minions[] = { //
		// Tier 1
		minion{id::alleycat, stats{1, 1}},
		minion{id::dire_wolf_alpha, stats{2, 2}},
		minion{id::dragonspawn_lieutenant, stats{2, 3}}.with_taunt(),
		minion{id::fiendish_servant, stats{2, 1}},
		minion{id::vulgar_homunculus, stats{2, 4}}.with_taunt(),
		minion{id::mecharoo, stats{1, 1}},
		minion{id::micro_machine, stats{1, 2}},
		minion{id::murloc_tidecaller, stats{1, 2}},
		minion{id::murloc_tidehunter, stats{2, 1}},
		minion{id::red_whelp, stats{1, 2}},
		minion{id::rockpool_hunter, stats{2, 3}},
		minion{id::righteous_protector, stats{1, 1}}.with_taunt().with_ds(),
		minion{id::selfless_hero, stats{2, 1}},
		minion{id::wrath_weaver, stats{1, 1}},
		// Tokens (tier 1)
		minion{id::big_bad_wolf, stats{3, 2}},
		minion{id::damaged_golem, stats{2, 1}},
		minion{id::finkle_einhorn, stats{3, 3}},
		minion{id::guard_bot, stats{2, 3}}.with_taunt(),
		minion{id::hyena, stats{2, 2}},
		minion{id::imp, stats{1, 1}},
		minion{id::ironhide_runt, stats{5, 5}},
		minion{id::jo_e_bot, stats{1, 1}},
		minion{id::microbot, stats{1, 1}},
		minion{id::murloc_scout, stats{1, 1}},
		minion{id::plant, stats{1, 1}},
		minion{id::rat, stats{1, 1}},
		minion{id::robosaur, stats{8, 8}},
		minion{id::spider, stats{1, 1}},
		minion{id::tabbycat, stats{1, 1}},
		minion{id::voidwalker, stats{1, 3}}.with_taunt(),
		minion{id::amalgam, stats{1, 1}},
		// Tier 2
		minion{id::glyph_guardian, stats{2, 4}},
		minion{id::harvest_golem, stats{2, 3}},
		minion{id::imprisoner, stats{3, 3}}.with_taunt(),
		minion{id::kaboom_bot, stats{2, 2}},
		minion{id::kindly_grandmother, stats{1, 1}},
		minion{id::metaltooth_leaper, stats{3, 3}},
		minion{id::murloc_warleader, stats{3, 3}},
		minion{id::nathrezim_overseer, stats{2, 3}},
		minion{id::old_murk_eye, stats{2, 4}},
		minion{id::pogo_hopper, stats{1, 1}},
		minion{id::rat_pack, stats{2, 2}},
		minion{id::scavenging_hyena, stats{2, 2}}.with_on_friendly_death(on_friendly_death::scavenging_hyena),
		minion{id::spawn_of_nzoth, stats{2, 2}},
		minion{id::steward_of_time, stats{3, 4}},
		minion{id::unstable_ghoul, stats{1, 3}}.with_taunt(),
		minion{id::waxrider_togwaggle, stats{1, 2}}.with_on_friendly_kill(on_friendly_kill::waxrider_togwaggle),
		minion{id::zoobot, stats{3, 3}},
		// Tier 3
		minion{id::bronze_warden, stats{2, 1}}.with_ds().with_reborn(),
		minion{id::cobalt_guardian, stats{6, 3}},
		minion{id::coldlight_seer, stats{2, 3}},
		minion{id::crowd_favorite, stats{4, 4}},
		minion{id::crystal_weaver, stats{5, 4}},
		minion{id::hangry_dragon, stats{4, 4}},
		minion{id::houndmaster, stats{4, 3}},
		minion{id::imp_gang_boss, stats{2, 4}}.with_on_damage(on_damage::imp_gang_boss),
		minion{id::infested_wolf, stats{3, 3}},
		minion{id::khadgar, stats{2, 2}},
		minion{id::pack_leader, stats{3, 3}},
		minion{id::piloted_shredder, stats{4, 3}},
		minion{id::replicating_menace, stats{3, 1}},
		minion{id::screwjank_clunker, stats{2, 5}},
		minion{id::shifter_zerus, stats{1, 1}},
		minion{id::soul_juggler, stats{3, 3}},
		minion{id::the_beast, stats{9, 7}},
		minion{id::twilight_emissary, stats{4, 4}}.with_taunt(),
		// Tier 4
		minion{id::annoy_o_module, stats{2, 4}}.with_ds().with_taunt(),
		minion{id::bolvar_fireblood, stats{1, 7}}.with_ds(),
		minion{id::cave_hydra, stats{2, 4}},
		minion{id::cobalt_scalebane, stats{5, 5}},
		minion{id::defender_of_argus, stats{2, 3}},
		minion{id::drakonid_enforcer, stats{3, 6}},
		minion{id::floating_watcher, stats{4, 4}},
		minion{id::herald_of_flame, stats{5, 6}}.with_on_attack(on_attack::herald_of_flame),
		minion{id::iron_sensei, stats{2, 2}},
		minion{id::mechano_egg, stats{0, 5}},
		minion{id::menagerie_magician, stats{4, 4}},
		minion{id::savannah_highmane, stats{6, 5}},
		minion{id::security_rover, stats{2, 6}}.with_on_damage(on_damage::security_rover),
		minion{id::siegebreaker, stats{5, 8}}.with_taunt(),
		minion{id::toxfin, stats{1, 2}},
		minion{id::virmen_sensei, stats{4, 5}},
		// Tier 5
		minion{id::annihilan_battlemaster, stats{3, 1}},
		minion{id::baron_rivendare, stats{1, 7}}.with_on_friendly_death(on_friendly_death::baron_rivendare),
		minion{id::brann_bronzebeard, stats{2, 4}},
		minion{id::goldrinn_the_great_wolf, stats{4, 4}},
		minion{id::ironhide_direhorn, stats{7, 7}}.with_on_attack(on_attack::ironhide_direhorn),
		minion{id::junkbot, stats{1, 5}}.with_on_friendly_death(on_friendly_death::junkbot),
		minion{id::king_bagurgle, stats{6, 3}},
		minion{id::lightfang_enforcer, stats{2, 2}},
		minion{id::malganis, stats{9, 7}},
		minion{id::murozond, stats{5, 5}},
		minion{id::primalfin_lookout, stats{3, 2}},
		minion{id::razoregore_the_untamed, stats{2, 4}},
		minion{id::sneeds_old_shredder, stats{5, 7}},
		minion{id::strongshell_scavenger, stats{2, 3}},
		minion{id::voidlord, stats{3, 9}}.with_taunt(),
		// Tier 6
		minion{id::foe_reaper_4000, stats{6, 9}},
		minion{id::gentle_megasaur, stats{5, 4}},
		minion{id::ghastcoiler, stats{7, 7}},
		minion{id::holy_mackerel, stats{8, 4}},
		minion{id::imp_mama, stats{6, 8}}.with_on_damage(on_damage::imp_mama),
		minion{id::kangors_apprentice, stats{3, 6}},
		minion{id::kalecgos_arcane_aspect, stats{4, 12}},
		minion{id::maexxna, stats{2, 8}}.with_poisonous(),
		minion{id::mama_bear, stats{5, 5}},
		minion{id::nadina_the_red, stats{7, 4}},
		minion{id::zapp_slywick, stats{7, 10}}.with_windfury()};

	/// Creates a fresh, optionally golden copy of the minion with the given @p id.
	auto create(id id, bool golden = false) -> minion;
}
