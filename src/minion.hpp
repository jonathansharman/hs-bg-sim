#pragma once

#include "names.hpp"
#include "tiers.hpp"
#include "tribes.hpp"

#include <unordered_map>
#include <vector>

struct stats {
	int attack;
	int health;
};

enum class golden { off, on };
enum class taunt { off, on };
enum class ds { off, on };
enum class reborn { off, on };
enum class poison { off, on };
enum class cleave { off, on };
enum class windfury { off, on, mega };

enum class dr {
	fiendish_servant,
	mecharoo,
	selfless_hero,
	harvest_golem,
	imprisoner,
	kaboom_bot,
	kindly_grandmother,
	rat_pack,
	spawn_of_nzoth,
	unstable_ghoul,
	infested_wolf,
	piloted_shredder,
	replicating_menace,
	the_beast,
	mechano_egg,
	savannah_highmane,
	goldrinn_the_great_wolf,
	king_bagurgle,
	sneeds_old_shredder,
	voidlord,
	ghastcoiler,
	kangors_apprentice,
	nadina_the_red
};
enum class start { red_whelp };
enum class on_attack { glyph_guardian, herald_of_flame, ironhide_direhorn, zapp_slywick };
enum class aura { murloc_warleader, old_murk_eye, siegebreaker, malganis };
enum class on_friendly_death { scavenging_hyena, baron_rivendare, junkbot };
enum class on_friendly_kill { waxrider_togwaggle };
enum class on_damage { imp_gang_boss, security_rover, imp_mama };
enum class on_summon { khadgar, pack_leader, mama_bear };
enum class on_lose_ds { drakonid_enforcer, lightfang_enforcer, holy_mackerel };

struct minion {
	name name;
	stats stats;

	golden golden = golden::off;
	taunt taunt = taunt::off;
	ds ds = ds::off;
	reborn reborn = reborn::off;
	poison poison = poison::off;
	cleave cleave = cleave::off;
	windfury windfury = windfury::off;

	std::vector<dr> drs;
	std::vector<start> starts;
	std::vector<on_attack> on_attacks;
	std::vector<aura> auras;
	std::vector<on_friendly_death> on_friendly_deaths;
	std::vector<on_friendly_kill> on_friendly_kills;
	std::vector<on_damage> on_damages;
	std::vector<on_summon> on_summons;
	std::vector<on_lose_ds> on_lose_dss;

	auto with_stats(int attack, int health) -> minion& {
		this->stats = {attack, health};
		return *this;
	}
	auto with_golden() -> minion& {
		golden = golden::on;
		return *this;
	}
	auto with_taunt() -> minion& {
		taunt = taunt::on;
		return *this;
	}
	auto with_ds() -> minion& {
		ds = ds::on;
		return *this;
	}
	auto with_reborn() -> minion& {
		reborn = reborn::on;
		return *this;
	}
	auto with_poison() -> minion& {
		poison = poison::on;
		return *this;
	}
	auto with_cleave() -> minion& {
		cleave = cleave::on;
		return *this;
	}
	auto with_windfury() -> minion& {
		windfury = windfury::on;
		return *this;
	}
	auto with_megawindfury() -> minion& {
		windfury = windfury::mega;
		return *this;
	}
	auto with_dr(dr dr) -> minion& {
		drs.push_back(dr);
		return *this;
	}
	auto with_start(start start) -> minion& {
		starts.push_back(start);
		return *this;
	}
	auto with_on_attack(on_attack on_attack) -> minion& {
		on_attacks.push_back(on_attack);
		return *this;
	}
	auto with_aura(aura aura) -> minion& {
		auras.push_back(aura);
		return *this;
	}
	auto with_on_friendly_death(on_friendly_death on_friendly_death) -> minion& {
		on_friendly_deaths.push_back(on_friendly_death);
		return *this;
	}
	auto with_on_friendly_kill(on_friendly_kill on_friendly_kill) -> minion& {
		on_friendly_kills.push_back(on_friendly_kill);
		return *this;
	}
	auto with_on_damage(on_damage on_damage) -> minion& {
		on_damages.push_back(on_damage);
		return *this;
	}
	auto with_on_summon(on_summon on_summon) -> minion& {
		on_summons.push_back(on_summon);
		return *this;
	}
	auto with_on_lose_ds(on_lose_ds on_lose_ds) -> minion& {
		on_lose_dss.push_back(on_lose_ds);
		return *this;
	}
};

auto get_minion_map() {
	std::unordered_map<name, minion> minion_map;
	auto add = [&minion_map](minion const& m) { minion_map.insert(std::make_pair(m.name, m)); };
	// Tier 1
	add(minion{name::alleycat, stats{1, 1}});
	add(minion{name::dire_wolf_alpha, stats{2, 2}});
	add(minion{name::dragonspawn_lieutenant, stats{2, 3}}.with_taunt());
	add(minion{name::fiendish_servant, stats{2, 1}}.with_dr(dr::fiendish_servant));
	add(minion{name::vulgar_homunculus, stats{2, 4}}.with_taunt());
	add(minion{name::mecharoo, stats{1, 1}}.with_dr(dr::mecharoo));
	add(minion{name::micro_machine, stats{1, 2}});
	add(minion{name::murloc_tidecaller, stats{1, 2}});
	add(minion{name::murloc_tidehunter, stats{2, 1}});
	add(minion{name::red_whelp, stats{1, 2}}.with_start(start::red_whelp));
	add(minion{name::rockpool_hunter, stats{2, 3}});
	add(minion{name::righteous_protector, stats{1, 1}}.with_taunt().with_ds());
	add(minion{name::selfless_hero, stats{2, 1}}.with_dr(dr::selfless_hero));
	add(minion{name::wrath_weaver, stats{1, 1}});
	// Tier 2
	add(minion{name::glyph_guardian, stats{2, 4}});
	add(minion{name::harvest_golem, stats{2, 3}}.with_dr(dr::harvest_golem));
	add(minion{name::imprisoner, stats{3 / 3}}.with_dr(dr::imprisoner));
	add(minion{name::kaboom_bot, stats{2, 2}}.with_dr(dr::kaboom_bot));
	add(minion{name::kindly_grandmother, stats{1, 1}}.with_dr(dr::kindly_grandmother));
	add(minion{name::metaltooth_leaper, stats{3, 3}});
	add(minion{name::murloc_warleader, stats{3, 3}}.with_aura(aura::murloc_warleader));
	add(minion{name::nathrezim_overseer, stats{2, 3}});
	add(minion{name::old_murk_eye, stats{2, 4}}.with_aura(aura::old_murk_eye));
	add(minion{name::pogo_hopper, stats{1, 1}});
	add(minion{name::rat_pack, stats{2, 2}}.with_dr(dr::rat_pack));
	add(minion{name::scavenging_hyena, stats{2, 2}}.with_on_friendly_death(on_friendly_death::scavenging_hyena));
	add(minion{name::spawn_of_nzoth, stats{2, 2}}.with_dr(dr::spawn_of_nzoth));
	add(minion{name::steward_of_time, stats{3, 4}});
	add(minion{name::unstable_ghoul, stats{1, 3}}.with_taunt().with_dr(dr::unstable_ghoul));
	add(minion{name::waxrider_togwaggle, stats{1, 2}}.with_on_friendly_kill(on_friendly_kill::waxrider_togwaggle));
	add(minion{name::zoobot, stats{3, 3}});
	// Tier 3
	add(minion{name::bronze_warden, stats{2, 1}}.with_ds().with_reborn());
	add(minion{name::cobalt_guardian, stats{6, 3}});
	add(minion{name::coldlight_seer, stats{2, 3}});
	add(minion{name::crowd_favorite, stats{4, 4}});
	add(minion{name::crystal_weaver, stats{5, 4}});
	add(minion{name::hangry_dragon, stats{4, 4}});
	add(minion{name::houndmaster, stats{4, 3}});
	add(minion{name::imp_gang_boss, stats{2, 4}}.with_on_damage(on_damage::imp_gang_boss));
	add(minion{name::infested_wolf, stats{3, 3}}.with_dr(dr::infested_wolf));
	add(minion{name::khadgar, stats{2, 2}}.with_on_summon(on_summon::khadgar));
	add(minion{name::pack_leader, stats{3, 3}}.with_on_summon(on_summon::pack_leader));
	add(minion{name::piloted_shredder, stats{4, 3}}.with_dr(dr::piloted_shredder));
	add(minion{name::replicating_menace, stats{3, 1}}.with_dr(dr::replicating_menace));
	add(minion{name::screwjank_clunker, stats{2, 5}});
	add(minion{name::shifter_zerus, stats{1, 1}});
	add(minion{name::soul_juggler, stats{3, 3}});
	add(minion{name::the_beast, stats{9, 7}}.with_dr(dr::the_beast));
	add(minion{name::twilight_emissary, stats{4, 4}}.with_taunt());
	// Tier 4
	add(minion{name::annoy_o_module, stats{2, 4}}.with_ds().with_taunt());
	add(minion{name::bolvar_fireblood, stats{1, 7}}.with_ds());
	add(minion{name::cave_hydra, stats{2, 4}}.with_cleave());
	add(minion{name::cobalt_scalebane, stats{5, 5}});
	add(minion{name::defender_of_argus, stats{2, 3}});
	add(minion{name::drakonid_enforcer, stats{3, 6}}.with_on_lose_ds(on_lose_ds::drakonid_enforcer));
	add(minion{name::floating_watcher, stats{4, 4}});
	add(minion{name::herald_of_flame, stats{5, 6}}.with_on_attack(on_attack::herald_of_flame));
	add(minion{name::iron_sensei, stats{2, 2}});
	add(minion{name::mechano_egg, stats{0, 5}}.with_dr(dr::mechano_egg));
	add(minion{name::menagerie_magician, stats{4, 4}});
	add(minion{name::savannah_highmane, stats{6, 5}}.with_dr(dr::savannah_highmane));
	add(minion{name::security_rover, stats{2, 6}}.with_on_damage(on_damage::security_rover));
	add(minion{name::siegebreaker, stats{5, 8}}.with_taunt().with_aura(aura::siegebreaker));
	add(minion{name::toxfin, stats{1, 2}});
	add(minion{name::virmen_sensei, stats{4, 5}});
	// Tier 5
	add(minion{name::annihilan_battlemaster, stats{3, 1}});
	add(minion{name::baron_rivendare, stats{1, 7}}.with_on_friendly_death(on_friendly_death::baron_rivendare));
	add(minion{name::brann_bronzebeard, stats{2, 4}});
	add(minion{name::goldrinn_the_great_wolf, stats{4, 4}}.with_dr(dr::goldrinn_the_great_wolf));
	add(minion{name::ironhide_direhorn, stats{7, 7}}.with_on_attack(on_attack::ironhide_direhorn));
	add(minion{name::junkbot, stats{1, 5}}.with_on_friendly_death(on_friendly_death::junkbot));
	add(minion{name::king_bagurgle, stats{6, 3}}.with_dr(dr::king_bagurgle));
	add(minion{name::lightfang_enforcer, stats{2, 2}});
	add(minion{name::malganis, stats{9, 7}}.with_aura(aura::malganis));
	add(minion{name::murozond, stats{5, 5}});
	add(minion{name::primalfin_lookout, stats{3, 2}});
	add(minion{name::razoregore_the_untamed, stats{2, 4}});
	add(minion{name::sneeds_old_shredder, stats{5, 7}}.with_dr(dr::sneeds_old_shredder));
	add(minion{name::strongshell_scavenger, stats{2, 3}});
	add(minion{name::voidlord, stats{3, 9}}.with_dr(dr::voidlord));
	// Tier 6
	add(minion{name::foe_reaper_4000, stats{6, 9}}.with_cleave());
	add(minion{name::gentle_megasaur, stats{5, 4}});
	add(minion{name::ghastcoiler, stats{7, 7}}.with_dr(dr::ghastcoiler));
	add(minion{name::holy_mackerel, stats{8, 4}}.with_on_lose_ds(on_lose_ds::holy_mackerel));
	add(minion{name::imp_mama, stats{6, 8}}.with_on_damage(on_damage::imp_mama));
	add(minion{name::kangors_apprentice, stats{3, 6}}.with_dr(dr::kangors_apprentice));
	add(minion{name::kalecgos_arcane_aspect, stats{4, 12}});
	add(minion{name::maexxna, stats{2, 8}}.with_poison());
	add(minion{name::mama_bear, stats{5, 5}}.with_on_summon(on_summon::mama_bear));
	add(minion{name::nadina_the_red, stats{7, 4}}.with_dr(dr::nadina_the_red));
	add(minion{name::zapp_slywick, stats{7, 10}}.with_windfury().with_on_attack(on_attack::zapp_slywick));
	// Tokens (tier 1)
	return minion_map;
}
