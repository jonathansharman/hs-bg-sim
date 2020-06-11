#include "tiers.hpp"

#include "minion.hpp"

namespace hsbg {
	auto get_tier(id id) -> tier {
		switch (id) {
			// Tier 1
			case id::alleycat:
			case id::dire_wolf_alpha:
			case id::dragonspawn_lieutenant:
			case id::fiendish_servant:
			case id::vulgar_homunculus:
			case id::mecharoo:
			case id::micro_machine:
			case id::murloc_tidecaller:
			case id::murloc_tidehunter:
			case id::red_whelp:
			case id::rockpool_hunter:
			case id::righteous_protector:
			case id::selfless_hero:
			case id::wrath_weaver:
			case id::big_bad_wolf:
			case id::damaged_golem:
			case id::finkle_einhorn:
			case id::guard_bot:
			case id::hyena:
			case id::imp:
			case id::ironhide_runt:
			case id::jo_e_bot:
			case id::microbot:
			case id::murloc_scout:
			case id::plant:
			case id::rat:
			case id::robosaur:
			case id::spider:
			case id::tabbycat:
			case id::voidwalker:
			case id::amalgam:
				return tier::one;
			// Tier 2
			case id::glyph_guardian:
			case id::harvest_golem:
			case id::imprisoner:
			case id::kaboom_bot:
			case id::kindly_grandmother:
			case id::metaltooth_leaper:
			case id::murloc_warleader:
			case id::nathrezim_overseer:
			case id::old_murk_eye:
			case id::pogo_hopper:
			case id::rat_pack:
			case id::scavenging_hyena:
			case id::spawn_of_nzoth:
			case id::steward_of_time:
			case id::unstable_ghoul:
			case id::waxrider_togwaggle:
			case id::zoobot:
				return tier::two;
			// Tier 3
			case id::bronze_warden:
			case id::cobalt_guardian:
			case id::coldlight_seer:
			case id::crowd_favorite:
			case id::crystal_weaver:
			case id::deflect_o_bot:
			case id::felfin_navigator:
			case id::hangry_dragon:
			case id::houndmaster:
			case id::imp_gang_boss:
			case id::infested_wolf:
			case id::khadgar:
			case id::pack_leader:
			case id::piloted_shredder:
			case id::replicating_menace:
			case id::screwjank_clunker:
			case id::shifter_zerus:
			case id::soul_juggler:
			case id::the_beast:
			case id::twilight_emissary:
				return tier::three;
			// Tier 4
			case id::annoy_o_module:
			case id::bolvar_fireblood:
			case id::cave_hydra:
			case id::cobalt_scalebane:
			case id::defender_of_argus:
			case id::drakonid_enforcer:
			case id::floating_watcher:
			case id::herald_of_flame:
			case id::iron_sensei:
			case id::mechano_egg:
			case id::menagerie_magician:
			case id::savannah_highmane:
			case id::security_rover:
			case id::siegebreaker:
			case id::toxfin:
			case id::virmen_sensei:
				return tier::four;
			// Tier 5
			case id::annihilan_battlemaster:
			case id::baron_rivendare:
			case id::brann_bronzebeard:
			case id::goldrinn_the_great_wolf:
			case id::ironhide_direhorn:
			case id::junkbot:
			case id::king_bagurgle:
			case id::lightfang_enforcer:
			case id::malganis:
			case id::murozond:
			case id::primalfin_lookout:
			case id::razorgore_the_untamed:
			case id::sneeds_old_shredder:
			case id::strongshell_scavenger:
			case id::voidlord:
				return tier::five;
			// Tier 6
			case id::foe_reaper_4000:
			case id::gentle_megasaur:
			case id::ghastcoiler:
			case id::holy_mackerel:
			case id::imp_mama:
			case id::kangors_apprentice:
			case id::kalecgos_arcane_aspect:
			case id::maexxna:
			case id::mama_bear:
			case id::nadina_the_red:
			case id::zapp_slywick:
				return tier::six;
		}
		// Unreachable.
		__assume(false);
	}

	auto get_tier(minion const& m) -> tier {
		return get_tier(m.id);
	}
}
