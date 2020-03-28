#include "names.hpp"

#include "minion.hpp"

auto get_name(id id) -> std::string {
	switch (id) {
		case id::alleycat:
			return "Alleycat";
		case id::dire_wolf_alpha:
			return "Dire Wolf Alpha";
		case id::dragonspawn_lieutenant:
			return "Dragonspawn Lieutenant";
		case id::fiendish_servant:
			return "Fiendish Servant";
		case id::vulgar_homunculus:
			return "Vulgar Homunculus";
		case id::mecharoo:
			return "Mecharoo";
		case id::micro_machine:
			return "Micro Machine";
		case id::murloc_tidecaller:
			return "Murloc Tidecaller";
		case id::murloc_tidehunter:
			return "Murloc Tidehunter";
		case id::red_whelp:
			return "Red Whelp";
		case id::rockpool_hunter:
			return "Rockpool Hunter";
		case id::righteous_protector:
			return "Righteous Protector";
		case id::selfless_hero:
			return "Selfless Hero";
		case id::wrath_weaver:
			return "Wrath Weaver";
		// Tokens (tier 1)
		case id::big_bad_wolf:
			return "Big Bad Wolf";
		case id::damaged_golem:
			return "Damaged Golem";
		case id::finkle_einhorn:
			return "Finkle Einhorn";
		case id::guard_bot:
			return "Guard Bot";
		case id::hyena:
			return "Hyena";
		case id::imp:
			return "Imp";
		case id::ironhide_runt:
			return "Ironhide Runt";
		case id::jo_e_bot:
			return "Jo-E Bot";
		case id::microbot:
			return "Microbot";
		case id::murloc_scout:
			return "Murloc Scout";
		case id::plant:
			return "Plant";
		case id::rat:
			return "Rat";
		case id::robosaur:
			return "Robosaur";
		case id::spider:
			return "Spider";
		case id::tabbycat:
			return "Tabbycat";
		case id::voidwalker:
			return "Voidwalker";
		case id::amalgam:
			return "Amalgam";
		// Tier 2
		case id::glyph_guardian:
			return "Glyph Guardian";
		case id::harvest_golem:
			return "Harvest Golem";
		case id::imprisoner:
			return "Imprisoner";
		case id::kaboom_bot:
			return "Kaboom Bot";
		case id::kindly_grandmother:
			return "Kindly Grandmother";
		case id::metaltooth_leaper:
			return "Metaltooth Leaper";
		case id::murloc_warleader:
			return "Murloc Warleader";
		case id::nathrezim_overseer:
			return "Nathrezim Overseer";
		case id::old_murk_eye:
			return "Old Murk-Eye";
		case id::pogo_hopper:
			return "Pogo-Hopper";
		case id::rat_pack:
			return "Rat Pack";
		case id::scavenging_hyena:
			return "Scavenging Hyena";
		case id::spawn_of_nzoth:
			return "Spawn of N'Zoth";
		case id::steward_of_time:
			return "Steward of Time";
		case id::unstable_ghoul:
			return "Unstable Ghoul";
		case id::waxrider_togwaggle:
			return "Waxrider Togwaggle";
		case id::zoobot:
			return "Zoobot";
		// Tier 3
		case id::bronze_warden:
			return "Bronze Warden";
		case id::cobalt_guardian:
			return "Cobalt Guardian";
		case id::coldlight_seer:
			return "Coldlight Seer";
		case id::crowd_favorite:
			return "Crowd Favorite";
		case id::crystal_weaver:
			return "Crystal Weaver";
		case id::hangry_dragon:
			return "Hangry Dragon";
		case id::houndmaster:
			return "Houndmaster";
		case id::imp_gang_boss:
			return "Imp Gang Boss";
		case id::infested_wolf:
			return "Infested Wolf";
		case id::khadgar:
			return "Khadgar";
		case id::pack_leader:
			return "Pack Leader";
		case id::piloted_shredder:
			return "Piloted Shredder";
		case id::replicating_menace:
			return "Replicating Menace";
		case id::screwjank_clunker:
			return "Screwjank Clunker";
		case id::shifter_zerus:
			return "Shifter Zerus";
		case id::soul_juggler:
			return "Soul Juggler";
		case id::the_beast:
			return "The Beast";
		case id::twilight_emissary:
			return "Twilight Emissary";
		// Tier 4
		case id::annoy_o_module:
			return "Annoy-o-Module";
		case id::bolvar_fireblood:
			return "Bolvar Fireblood";
		case id::cave_hydra:
			return "Cave Hydra";
		case id::cobalt_scalebane:
			return "Cobalt Scalebane";
		case id::defender_of_argus:
			return "Defender of Argus";
		case id::drakonid_enforcer:
			return "Drakonid Enforcer";
		case id::floating_watcher:
			return "Floating Watcher";
		case id::herald_of_flame:
			return "Herald of Flame";
		case id::iron_sensei:
			return "Iron Sensei";
		case id::mechano_egg:
			return "Mechano-Egg";
		case id::menagerie_magician:
			return "Menagerie Magician";
		case id::savannah_highmane:
			return "Savannah Highmane";
		case id::security_rover:
			return "Security Rover";
		case id::siegebreaker:
			return "Siegebreaker";
		case id::toxfin:
			return "Toxfin";
		case id::virmen_sensei:
			return "Virmen Sensei";
		// Tier 5
		case id::annihilan_battlemaster:
			return "Annihilan Battlemaster";
		case id::baron_rivendare:
			return "Baron Rivendare";
		case id::brann_bronzebeard:
			return "Brann Bronzebeard";
		case id::goldrinn_the_great_wolf:
			return "Goldrinn, the Great Wolf";
		case id::ironhide_direhorn:
			return "Ironhide Direhorn";
		case id::junkbot:
			return "Junkbot";
		case id::king_bagurgle:
			return "King Bagurgle";
		case id::lightfang_enforcer:
			return "Lightfang Enforcer";
		case id::malganis:
			return "Mal'Ganis";
		case id::murozond:
			return "Murozond";
		case id::primalfin_lookout:
			return "Primalfin Lookout";
		case id::razoregore_the_untamed:
			return "Razoregore, the Untamed";
		case id::sneeds_old_shredder:
			return "Sneed's Old Shredder";
		case id::strongshell_scavenger:
			return "Strongshell Scavenger";
		case id::voidlord:
			return "Voidlord";
		// Tier 6
		case id::foe_reaper_4000:
			return "Foe Reaper 4000";
		case id::gentle_megasaur:
			return "Gentle Megasaur";
		case id::ghastcoiler:
			return "Ghastcoiler";
		case id::holy_mackerel:
			return "Holy Mackerel";
		case id::imp_mama:
			return "Imp Mama";
		case id::kangors_apprentice:
			return "Kangor's Apprentice";
		case id::kalecgos_arcane_aspect:
			return "Kalecgos, Arcane Aspect";
		case id::maexxna:
			return "Maexxna";
		case id::mama_bear:
			return "Mama Bear";
		case id::nadina_the_red:
			return "Nadina the Red";
		case id::zapp_slywick:
			return "Zapp Slywick";
		default:
			// Unreachable.
			__assume(false);
	}
}

auto get_name(minion const& m) -> std::string {
	return get_name(m.id);
}
