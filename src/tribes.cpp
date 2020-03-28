#include "tribes.hpp"

#include "minion.hpp"

inline auto get_tribe(id const& id) -> tribe {
	switch (id) {
		case id::baron_rivendare:
		case id::bolvar_fireblood:
		case id::brann_bronzebeard:
		case id::crowd_favorite:
		case id::crystal_weaver:
		case id::defender_of_argus:
		case id::finkle_einhorn:
		case id::houndmaster:
		case id::kangors_apprentice:
		case id::khadgar:
		case id::lightfang_enforcer:
		case id::menagerie_magician:
		case id::nadina_the_red:
		case id::pack_leader:
		case id::plant:
		case id::righteous_protector:
		case id::selfless_hero:
		case id::shifter_zerus:
		case id::soul_juggler:
		case id::spawn_of_nzoth:
		case id::strongshell_scavenger:
		case id::unstable_ghoul:
		case id::virmen_sensei:
		case id::waxrider_togwaggle:
		case id::wrath_weaver:
		case id::zapp_slywick:
			return tribe::none;
		case id::annoy_o_module:
		case id::damaged_golem:
		case id::foe_reaper_4000:
		case id::guard_bot:
		case id::harvest_golem:
		case id::iron_sensei:
		case id::jo_e_bot:
		case id::junkbot:
		case id::kaboom_bot:
		case id::mechano_egg:
		case id::mecharoo:
		case id::metaltooth_leaper:
		case id::micro_machine:
		case id::microbot:
		case id::piloted_shredder:
		case id::pogo_hopper:
		case id::robosaur:
		case id::replicating_menace:
		case id::screwjank_clunker:
		case id::security_rover:
		case id::sneeds_old_shredder:
		case id::zoobot:
			return tribe::mech;
		case id::alleycat:
		case id::big_bad_wolf:
		case id::cave_hydra:
		case id::dire_wolf_alpha:
		case id::gentle_megasaur:
		case id::ghastcoiler:
		case id::goldrinn_the_great_wolf:
		case id::hyena:
		case id::infested_wolf:
		case id::ironhide_direhorn:
		case id::ironhide_runt:
		case id::kindly_grandmother:
		case id::maexxna:
		case id::mama_bear:
		case id::rat:
		case id::rat_pack:
		case id::savannah_highmane:
		case id::scavenging_hyena:
		case id::spider:
		case id::tabbycat:
		case id::the_beast:
			return tribe::beast;
		case id::annihilan_battlemaster:
		case id::fiendish_servant:
		case id::floating_watcher:
		case id::imp:
		case id::imp_gang_boss:
		case id::imp_mama:
		case id::imprisoner:
		case id::malganis:
		case id::nathrezim_overseer:
		case id::siegebreaker:
		case id::voidlord:
		case id::voidwalker:
		case id::vulgar_homunculus:
			return tribe::demon;
		case id::coldlight_seer:
		case id::holy_mackerel:
		case id::king_bagurgle:
		case id::murloc_scout:
		case id::murloc_tidecaller:
		case id::murloc_tidehunter:
		case id::murloc_warleader:
		case id::old_murk_eye:
		case id::primalfin_lookout:
		case id::rockpool_hunter:
		case id::toxfin:
			return tribe::murloc;
		case id::bronze_warden:
		case id::cobalt_guardian:
		case id::cobalt_scalebane:
		case id::dragonspawn_lieutenant:
		case id::drakonid_enforcer:
		case id::glyph_guardian:
		case id::hangry_dragon:
		case id::herald_of_flame:
		case id::kalecgos_arcane_aspect:
		case id::murozond:
		case id::razoregore_the_untamed:
		case id::red_whelp:
		case id::steward_of_time:
		case id::twilight_emissary:
			return tribe::dragon;
		case id::amalgam:
			return tribe::all;
	}
	// Unreachable.
	__assume(false);
}

auto get_tribe(minion const& m) -> tribe {
	return get_tribe(m.id);
}
