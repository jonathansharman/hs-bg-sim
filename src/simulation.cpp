#include "simulation.hpp"

#include "random.hpp"
#include "tiers.hpp"
#include "tribes.hpp"

#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <range/v3/algorithm/permutation.hpp>
#include <range/v3/algorithm/sort.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip_with.hpp>

#include <future>
#include <numeric>
#include <optional>

namespace hsbg {
	namespace {
		template <typename Filter>
		auto filtered_its(warband& wb, Filter const& f) {
			std::vector<wb_it> its;
			for (auto it = wb.begin(); it != wb.end(); ++it) {
				if (f(*it)) { its.push_back(it); }
			}
			return its;
		}

		auto choose(std::vector<wb_it> const& its) -> std::optional<wb_it> {
			return its.empty() ? std::nullopt : std::make_optional(rand_element(its));
		}

		auto choose_alive(warband& wb) -> std::optional<wb_it> {
			return choose(filtered_its(wb, [](minion const& m) { return m.stats.alive(); }));
		}

		/// Simulates HS BG combat.
		struct simulation {
			/// Builds a simulation and runs it to completion.
			/// @param trace If true, intermediate board states will be printed.
			simulation(board board, bool trace = false);

			/// The final result of this simulation.
			auto results() const -> combat_results {
				return _results;
			}

		private:
			/// Current board state.
			board _board;

			/// Tracks which minion should attack next, if possible.
			std::array<wb_it, 2> _current_attackers;

			/// Used for Kangor's Apprentice's deathrattle.
			std::array<std::vector<minion>, 2> _dead_mechs_lists;

			combat_results _results;

			/// Gets a reference to the warband to which the passed minion belongs.
			/// @note Behavior is undefined if the minion is not present in either warband.
			auto get_allies(wb_it target) -> warband&;

			/// Gets a reference to the warband opposing the input warband.
			auto get_enemies(warband& wb) -> warband&;

			/// Gets a reference to the list of dead friendly mechs for this warband.
			auto get_dead_mechs(warband& wb) -> std::vector<minion>&;

			/// Summons @p summoned to @p target in @p allies, as space permits.
			/// @param summoned_by Reference to the warband that caused this summon.
			/// @param khadgars The set of Khadgars that have already participated in this summoning.
			/// @return An iterator one past @p target, if summoning was successful, or @p target otherwise.
			auto summon( //
				warband& allies,
				wb_it target,
				minion summoned,
				warband const& summoned_by,
				std::unordered_set<minion*> khadgars) -> wb_it;

			/// Invokes @p generator @p count times and summons them to @p target, as space permits.
			auto summon_n( //
				int count,
				warband& allies,
				wb_it target,
				std::function<minion()> const& generator,
				warband const& summoned_by) -> void;

			/// Summons @p count copies of @p summoned at @p target, as space permits.
			auto summon_n( //
				int count,
				warband& allies,
				wb_it target,
				minion const& summoned,
				warband const& summoned_by) -> void;

			/// Triggers the deathrattle of the given minion.
			auto trigger_dr(wb_it dying_it) -> void;

			/// Chooses a random attack target based on the given attacking warband and minion indices.
			auto choose_target(wb_it attacker) -> std::optional<wb_it>;

			/// Causes @p target to take @p damage from @p source.
			/// @param can_overkill Determines whether this can trigger overkill effects.
			auto take_damage_from(wb_it target, int damage, wb_it source, bool can_overkill = true) -> void;

			/// Executes the given attack.
			auto attack(wb_it attacker, wb_it primary_target) -> void;

			/// Handles deathrattles, reborn, etc.
			auto resolve_deaths() -> void;
		};
	}

	simulation::simulation(board board, bool trace) : _board{std::move(board)} {
		if (trace) { pretty_print(_board); }

		std::size_t starting_wb =
			(_board[0].size() > _board[1].size() || _board[0].size() == _board[1].size() && rand_int(0, 1)) ? 0 : 1;
		std::size_t active_wb_idx = starting_wb;
		_current_attackers = {_board[0].begin(), _board[1].begin()};

		// @todo Start-of-combat effects.
		// auto is_red_whelp = [](minion const& m) { return m.id == id::red_whelp; };
		// auto it0 = std::find_if(wbs[0].begin(), wbs[0].end(), is_red_whelp);
		// auto it1 = std::find_if(wbs[1].begin(), wbs[1].end(), is_red_whelp);

		// Perform attacks until one side is dead.
		bool previous_warband_passed = false;
		while (std::none_of(_board.begin(), _board.end(), [](warband const& wb) { return wb.empty(); })) {
			// Whether the current warband is passing.
			bool current_warband_passed = true;

			auto const find_attacker = [&]() -> std::optional<wb_it> {
				if (_board[active_wb_idx].empty()) {
					// No minions left.
					return std::nullopt;
				}
				// Search from current attacker to the right.
				for (wb_it attacker = _current_attackers[active_wb_idx]; attacker != _board[active_wb_idx].end(); ++attacker) {
					if (attacker->stats.alive() && attacker->stats.attack() > 0) {
						_current_attackers[active_wb_idx] = std::next(attacker);
						return attacker;
					}
				}
				// No viable attackers found towards the right. Wrap back around.
				for (wb_it attacker = _board[active_wb_idx].begin(); attacker != _current_attackers[active_wb_idx]; ++attacker) {
					if (attacker->stats.alive() && attacker->stats.attack() > 0) {
						_current_attackers[active_wb_idx] = std::next(attacker);
						return attacker;
					}
				}
				// No viable attackers from beginning to current attacker.
				return std::nullopt;
			};

			auto const o_attacker = find_attacker();
			if (o_attacker) {
				auto const attacker = *o_attacker;
				current_warband_passed = false;
				for (int i = 0; i < attacker->attack_count && attacker->stats.alive(); ++i) {
					auto const o_target = choose_target(attacker);
					if (o_target) {
						attack(attacker, *o_target);
						resolve_deaths();
					}
				}
				// Remove dead minions.
				for (std::size_t i = 0; i < 2; ++i) {
					warband& wb = _board[i];
					wb_it& current_attacker = _current_attackers[i];
					for (auto it = wb.begin(); it != wb.end();) {
						if (it->stats.dead()) {
							// If the current attacker has died, need to move the current attacker forward by one.
							if (it == current_attacker) {
								++current_attacker;
								if (current_attacker == wb.end()) { current_attacker = wb.begin(); }
							}
							// Erase the dead minion.
							it = wb.erase(it);
						} else {
							// Go to next minion.
							++it;
						}
					}
				}
			}

			// Switch control to the other warband.
			active_wb_idx = 1 - active_wb_idx;

			if (trace) { pretty_print(_board); }

			// End combat if both warbands are unable to attack in a row.
			if (current_warband_passed && previous_warband_passed) {
				break;
			} else {
				previous_warband_passed = current_warband_passed;
			}
		}
		// Compute results.
		_results.n_trials = 1;
		_results.allied_score = std::reduce(_board[0].begin(), _board[0].end(), 0, [](int sum, minion const& m) {
			return sum + static_cast<int>(get_tier(m.id));
		});
		_results.enemy_score = std::reduce(_board[1].begin(), _board[1].end(), 0, [](int sum, minion const& m) {
			return sum + static_cast<int>(get_tier(m.id));
		});
		int const net_score = _results.net_score();
		if (net_score > 0) {
			++_results.wins;
		} else if (net_score < 0) {
			++_results.losses;
		} else {
			++_results.draws;
		}
	}

	auto simulation::get_allies(wb_it target) -> warband& {
		for (auto& wb : _board) {
			for (auto it = wb.begin(); it != wb.end(); ++it) {
				if (&*it == &*target) { return wb; }
			}
		}
		// Unreachable.
		__assume(false);
	}

	auto simulation::get_enemies(warband& wb) -> warband& {
		return &wb == &_board[0] ? _board[1] : _board[0];
	}

	auto simulation::get_dead_mechs(warband& wb) -> std::vector<minion>& {
		return &wb == &_board[0] ? _dead_mechs_lists[0] : _dead_mechs_lists[1];
	}

	auto simulation::summon( //
		warband& allies,
		wb_it target,
		minion summoned,
		warband const& summoned_by,
		std::unordered_set<minion*> khadgars) -> wb_it { //
		auto const live_count = std::count_if(
			allies.begin(), allies.end(), [](minion const& m) { return m.stats.alive(); });
		if (live_count < 7) {
			// Apply auras.
			for (auto const& ally : allies) {
				switch (ally.id) {
					case id::murloc_warleader:
						if (is_murloc(summoned)) { summoned.stats.buff_attack(2); }
						break;
					case id::siegebreaker:
						if (is_demon(summoned)) { summoned.stats.buff_attack(1); }
						break;
					case id::malganis:
						if (is_demon(summoned)) {
							summoned.stats.buff_attack(2);
							summoned.stats.buff_health(2);
						}
						break;
					default:
						// Ally has no aura.
						break;
				}
			}

			// Trigger on-summon effects.
			for (auto& wb : _board) {
				bool const allied = &wb == &allies;
				for (auto it = wb.begin(); it != wb.end(); ++it) {
					switch (it->id) {
						case id::old_murk_eye:
							// Gains 1 (golden: 2) attack when a new murloc appears anywhere.
							if (is_murloc(summoned)) { it->stats.buff_attack(it->golden ? 2 : 1); }
							break;
						case id::cobalt_guardian:
							// Gains divine shield when friendly mechs are summoned.
							if (allied && is_mech(summoned)) { it->ds = true; }
							break;
						case id::deflect_o_bot:
							// Gains divine shield and +1 (golden: +2) attack when friendly mechs are summoned.
							if (allied && is_mech(summoned)) {
								it->stats.buff_attack(it->golden ? 2 : 1);
								it->ds = true;
							}
							break;
						case id::khadgar: {
							// Khadgar triggers summons twice (golden: three times) if summoned by a friendly source.
							// Each Khadgar on the board can only contibute additional summons once per normal summon.
							auto& khadgar_allies = get_allies(it);
							if (&summoned_by == &khadgar_allies && khadgars.find(&*it) == khadgars.end()) {
								for (int i = 0; i < (it->golden ? 2 : 1); ++i) {
									/// @todo Look up correct rules for where to put Khadgar summons.
									khadgars.insert(&*it);
									summon(allies, target, summoned, khadgar_allies, khadgars);
								}
							}
							break;
						}
						case id::pack_leader:
							// Gives friendly summoned beasts +3 attack (golden: +6 attack).
							if (allied && is_beast(summoned)) { summoned.stats.buff_attack(it->golden ? 6 : 3); }
							break;
						case id::mama_bear:
							// Gives friendly summoned beasts +4/+4 (golden: +8/+8).
							if (allied && is_beast(summoned)) {
								int const buff = it->golden ? 8 : 4;
								summoned.stats.buff_attack(buff);
								summoned.stats.buff_health(buff);
							}
							break;
						default:
							// No on-summon effect.
							break;
					}
				}
			}
			// Insert the summoned minion.
			return std::next(allies.insert(target, summoned));
		} else {
			return target;
		}
	}

	auto simulation::summon_n( //
		int count,
		warband& allies,
		wb_it target,
		std::function<minion()> const& generator,
		warband const& summoned_by) -> void //
	{
		for (int i = 0; i < count; ++i) {
			target = summon(allies, target, generator(), summoned_by, {});
		}
	}

	auto simulation::summon_n( //
		int count,
		warband& allies,
		wb_it target,
		minion const& summoned,
		warband const& summoned_by) -> void //
	{
		for (int i = 0; i < count; ++i) {
			target = summon(allies, target, summoned, summoned_by, {});
		}
	}

	auto simulation::trigger_dr(wb_it dying_it) -> void {
		auto& allies = get_allies(dying_it);
		auto& enemies = get_enemies(allies);
		// Trigger card-text deathrattles.
		switch (dying_it->id) {
			case id::fiendish_servant:
				// Add attack to another random minion (golden: two minions, repeats allowed).
				for (int i = 0; i < (dying_it->golden ? 2 : 1); ++i) {
					auto recipient = choose_alive(allies);
					if (recipient) { (*recipient)->stats.buff_attack(dying_it->stats.attack()); }
				}
				break;
			case id::mecharoo:
				summon(allies, std::next(dying_it), create(id::jo_e_bot, dying_it->golden), allies, {});
				break;
			case id::selfless_hero: {
				// Give divine shield to one (golden: two) random minions without divine shield already.
				for (int i = 0; i < (dying_it->golden ? 2 : 1); ++i) {
					auto recipient = choose(filtered_its(allies, [](minion const& m) { return m.stats.alive() && !m.ds; }));
					if (recipient) { (*recipient)->ds = true; }
				}
				break;
			}
			case id::harvest_golem:
				summon(allies, std::next(dying_it), create(id::damaged_golem, dying_it->golden), allies, {});
				break;
			case id::imprisoner:
				summon(allies, std::next(dying_it), create(id::imp, dying_it->golden), allies, {});
				break;
			case id::kaboom_bot:
				// Deal 4 damage to a random enemy minion (golden: twice).
				for (int i = 0; i < (dying_it->golden ? 2 : 1); ++i) {
					if (auto target = choose_alive(get_enemies(allies))) { take_damage_from(*target, 4, dying_it); }
				}
				break;
			case id::kindly_grandmother:
				summon(allies, std::next(dying_it), create(id::big_bad_wolf, dying_it->golden), allies, {});
				break;
			case id::rat_pack:
				// Summon rats (golden: golden rats) equal to the rat pack's attack.
				summon_n(dying_it->stats.attack(), allies, std::next(dying_it), create(id::rat, dying_it->golden), allies);
				break;
			case id::spawn_of_nzoth:
				// Give all other minions +1/+1 (golden: +2/+2).
				for (auto& ally : allies) {
					int const buff = dying_it->golden ? 2 : 1;
					if (&ally != &*dying_it) {
						ally.stats.buff_attack(buff);
						ally.stats.buff_health(buff);
					}
				}
				break;
			case id::unstable_ghoul:
				// Deal 1 (golden: 2) damage to all other minions.
				for (auto& warband : _board) {
					for (auto it = warband.begin(); it != warband.end(); ++it) {
						if (&*it != &*dying_it) { take_damage_from(it, dying_it->golden ? 2 : 1, dying_it); }
					}
				}
				break;
			case id::infested_wolf:
				// Summon two spiders (golden: golden spiders).
				summon_n(2, allies, std::next(dying_it), create(id::spider, dying_it->golden), allies);
				break;
			case id::piloted_shredder: {
				// Summon one random 2-cost minion (golden: two minions).
				static id two_costs[] = {//
					id::dire_wolf_alpha,
					id::vulgar_homunculus,
					id::micro_machine,
					id::murloc_tidehunter,
					id::rockpool_hunter,
					id::dragonspawn_lieutenant,
					id::kindly_grandmother,
					id::scavenging_hyena,
					id::unstable_ghoul,
					id::khadgar};
				summon_n(
					2, allies, std::next(dying_it), [] { return create(rand_element(two_costs)); }, allies);
				break;
			}
			case id::replicating_menace:
				// Summon three (golden: golden) microbots.
				summon_n(3, allies, std::next(dying_it), create(id::microbot, dying_it->golden), allies);
				break;
			case id::the_beast:
				// Summon a 3/3 for the opponent (not affected by golden).
				summon(enemies, enemies.end(), create(id::finkle_einhorn), allies, {});
				break;
			case id::mechano_egg:
				summon(allies, std::next(dying_it), create(id::robosaur, dying_it->golden), allies, {});
				break;
			case id::savannah_highmane:
				// Summon two (golden: golden) hyenas.
				summon_n(2, allies, std::next(dying_it), create(id::hyena, dying_it->golden), allies);
				break;
			case id::goldrinn_the_great_wolf:
				// Give allied beasts +4/+4 (golden: +8/+8).
				for (auto ally = allies.begin(); ally != allies.end(); ++ally) {
					int const buff = dying_it->golden ? 8 : 4;
					if (ally != dying_it && get_tribe(*ally) == tribe::beast) {
						ally->stats.buff_attack(buff);
						ally->stats.buff_health(buff);
					}
				}
				break;
			case id::king_bagurgle:
				// Give allied murlocs +2/+2 (golden: +4/+4).
				for (auto ally = allies.begin(); ally != allies.end(); ++ally) {
					int const buff = dying_it->golden ? 4 : 2;
					if (ally != dying_it && get_tribe(*ally) == tribe::murloc) {
						ally->stats.buff_attack(buff);
						ally->stats.buff_health(buff);
					}
				}
				break;
			case id::sneeds_old_shredder: {
				// Summon one random legendary minion (golden: two minions).
				static id sneeds_summons[] = {//
					id::old_murk_eye,
					id::waxrider_togwaggle,
					id::khadgar,
					id::shifter_zerus,
					id::the_beast,
					id::bolvar_fireblood,
					id::baron_rivendare,
					id::brann_bronzebeard,
					id::goldrinn_the_great_wolf,
					id::king_bagurgle,
					id::murozond,
					id::malganis,
					id::razoregore_the_untamed,
					id::foe_reaper_4000,
					id::kalecgos_arcane_aspect,
					id::maexxna,
					id::nadina_the_red,
					id::zapp_slywick};
				summon_n(
					dying_it->golden ? 2 : 1,
					allies,
					std::next(dying_it),
					[] { return create(rand_element(sneeds_summons)); },
					allies);
				break;
			}
			case id::voidlord:
				// Summon three (golden: golden) voidwalkers.
				summon_n(3, allies, std::next(dying_it), create(id::voidwalker, dying_it->golden), allies);
				break;
			case id::ghastcoiler: {
				// Summon two random deathrattle minions (golden: four minions).
				static id ghastcoiler_summons[] = {//
					id::fiendish_servant,
					id::mecharoo,
					id::selfless_hero,
					id::harvest_golem,
					id::imprisoner,
					id::kaboom_bot,
					id::kindly_grandmother,
					id::rat_pack,
					id::spawn_of_nzoth,
					id::unstable_ghoul,
					id::infested_wolf,
					id::piloted_shredder,
					id::replicating_menace,
					id::the_beast,
					id::mechano_egg,
					id::savannah_highmane,
					id::goldrinn_the_great_wolf,
					id::king_bagurgle,
					id::sneeds_old_shredder,
					id::voidlord,
					id::kangors_apprentice,
					id::nadina_the_red};
				summon_n(
					dying_it->golden ? 4 : 2,
					allies,
					std::next(dying_it),
					[] { return create(rand_element(ghastcoiler_summons)); },
					allies);
				break;
			}
			case id::kangors_apprentice: {
				// Summon up to two (golden: four) of the first friendly mechs that died this combat.
				auto const& dead_mechs = get_dead_mechs(allies);
				int const count = std::min(static_cast<int>(dead_mechs.size()), dying_it->golden ? 4 : 2);
				int i = 0;
				summon_n(
					count,
					allies,
					std::next(dying_it),
					[&dead_mechs, &i] {
						auto result = create(dead_mechs[i].id, dead_mechs[i].golden);
						++i;
						return result;
					},
					allies);
				break;
			}
			case id::nadina_the_red:
				// Give allied dragons divine shield.
				for (auto ally = allies.begin(); ally != allies.end(); ++ally) {
					if (ally != dying_it && get_tribe(*ally) == tribe::dragon) { ally->ds = true; }
				}
				break;
			default:
				// No deathrattle on card.
				break;
		}
		// Trigger conferred deathrattles.
		for (dr dr : dying_it->drs) {
			switch (dr) {
				case dr::microbots:
					summon_n(3, allies, std::next(dying_it), create(id::microbot), allies);
					break;
				case dr::golden_microbots:
					summon_n(3, allies, std::next(dying_it), create(id::microbot, true), allies);
					break;
				case dr::plants:
					summon_n(2, allies, std::next(dying_it), create(id::plant), allies);
					break;
			}
		}
		{ // Update dead mechs list.
			auto& dead_mechs = get_dead_mechs(allies);
			if (dead_mechs.size() < 4 && get_tribe(*dying_it) == tribe::mech) {
				dead_mechs.push_back(create(dying_it->id, dying_it->golden));
			}
		}
	}

	auto simulation::choose_target(wb_it attacker) -> std::optional<wb_it> {
		std::vector<wb_it> targets;
		warband& enemies = get_enemies(get_allies(attacker));
		if (attacker->id == id::zapp_slywick) {
			// Zapp Slywick prioritizes low-health minions.
			int min_health = enemies.front().stats.health();
			targets.push_back(enemies.begin());
			for (auto it = std::next(enemies.begin()); it != enemies.end(); ++it) {
				int const health = it->stats.health();
				if (health < min_health) {
					min_health = health;
					targets.clear();
				}
				if (health <= min_health) { targets.push_back(it); }
			}
		} else {
			// Everyone except Zapp prioritizes taunts.
			bool found_taunt = false;
			for (auto it = enemies.begin(); it != enemies.end(); ++it) {
				bool const taunt = it->taunt;
				if (taunt && !found_taunt) {
					found_taunt = true;
					targets.clear();
				}
				if (taunt || !found_taunt) { targets.push_back(it); }
			}
		}
		// Select target at random from valid targets.
		return targets.empty() ? std::nullopt : std::make_optional(rand_element(targets));
	}

	auto simulation::take_damage_from(wb_it target, int damage, wb_it source, bool can_overkill) -> void {
		if (damage == 0) {
			// Dealing zero damage does absolutely nothing.
			return;
		}
		auto& allies = get_allies(target);
		auto& enemies = get_enemies(allies);
		if (target->ds) {
			target->ds = false;
			// Trigger loss-of-divine-shield effects.
			for (auto& ally : allies) {
				if (&ally != &*target) {
					switch (ally.id) {
						case id::bolvar_fireblood:
							ally.stats.buff_attack(ally.golden ? 4 : 2);
							break;
						case id::drakonid_enforcer: {
							int const buff = ally.golden ? 4 : 2;
							ally.stats.buff_attack(buff);
							ally.stats.buff_health(buff);
							break;
						}
						case id::holy_mackerel:
							ally.ds = true;
							break;
						default:
							// No loss-of-divine-shield effects.
							break;
					}
				}
			}
		} else {
			target->stats.lose_health(damage);
			// Trigger on-damage effects.
			switch (target->id) {
				case id::imp_gang_boss:
					summon(allies, std::next(target), create(id::imp, target->golden), allies, {});
					break;
				case id::security_rover:
					summon(allies, std::next(target), create(id::guard_bot, target->golden), allies, {});
					break;
				case id::imp_mama: {
					// Summon one (golden: two) random demons. Cannot summon itself.
					static id imp_mama_summons[] = {//
						id::fiendish_servant,
						id::vulgar_homunculus,
						id::imprisoner,
						id::nathrezim_overseer,
						id::imp_gang_boss,
						id::floating_watcher,
						id::siegebreaker,
						id::annihilan_battlemaster,
						id::malganis,
						id::voidlord};
					summon_n(
						target->golden ? 2 : 1,
						allies,
						std::next(target),
						[] { return create(rand_element(imp_mama_summons)); },
						allies);
					break;
				}
				default:
					// No on-damage effects.
					break;
			}
			// Determine if the target is now dying.
			if (source->poisonous) {
				target->stats.poison();
				if (target->stats.alive()) { target->stats.make_dying(); }
			}
			if (target->stats.alive() && target->stats.health() <= 0) {
				target->stats.make_dying();
				if (can_overkill && target->stats.health() < 0) {
					// Trigger overkill effects.
					switch (source->id) {
						case id::herald_of_flame: {
							// Deal damage to left-most living minion in the target's warband.
							auto const leftmost_living = std::find_if( //
								allies.begin(),
								allies.end(),
								[](minion const& ally) { return ally.stats.alive(); });
							if (leftmost_living != allies.end()) {
								take_damage_from(leftmost_living, source->golden ? 6 : 3, source);
							}
							break;
						}
						case id::ironhide_direhorn: {
							summon(enemies, std::next(source), create(id::ironhide_runt, source->golden), enemies, {});
							break;
						}
						default:
							// No overkill effects.
							break;
					}
				}
			}
			if (!target->stats.alive()) {
				// Trigger on-kill effects.
				for (auto it = enemies.begin(); it != enemies.end(); ++it) {
					if (it->id == id::waxrider_togwaggle && get_tribe(*source) == tribe::dragon) {
						int const buff = it->golden ? 4 : 2;
						it->stats.buff_attack(buff);
						it->stats.buff_health(buff);
					}
				}
			}
		}
	}

	auto simulation::attack(wb_it attacker, wb_it primary_target) -> void {
		auto& enemies = get_allies(primary_target);
		if (attacker->id == id::glyph_guardian) {
			// Trigger Glyph Guardian effect.
			attacker->stats.buff_attack((attacker->golden ? 2 : 1) * attacker->stats.attack());
		}
		// Deal damage.
		auto const cleave = attacker->id == id::cave_hydra || attacker->id == id::foe_reaper_4000;
		// Build target list.
		std::vector<wb_it> targets;
		if (cleave && primary_target != enemies.begin()) {
			// Cleave targets left neighbor.
			targets.push_back(std::prev(primary_target));
		}
		targets.push_back(primary_target);
		if (cleave && primary_target != std::prev(enemies.end())) {
			// Cleave targets right neighbor.
			targets.push_back(std::next(primary_target));
		}
		// Hit targets.
		for (auto target : targets) {
			take_damage_from(target, attacker->stats.attack(), attacker);
		}
		// Reciprocate against attacker. Such damage cannot trigger overkill.
		take_damage_from(attacker, primary_target->stats.attack(), primary_target, false);
	}

	auto simulation::resolve_deaths() -> void {
		bool repeat = false;
		for (auto& wb : _board) {
			/// @todo How is it determined which player's minion deaths resolve first?
			for (auto it = wb.begin(); it != wb.end(); ++it) {
				// Only interested in minions marked as dying.
				if (!it->stats.dying()) { continue; }
				// A minion has died. Will need to perform death resolution again.
				repeat = true;
				// Trigger on-other-death effects of other minions.
				for (auto& other_wb : _board) {
					for (auto other_it = other_wb.begin(); other_it != other_wb.end(); ++other_it) {
						if (&*other_it == &*it) { continue; }
						switch (other_it->id) {
							case id::old_murk_eye:
								if (get_tribe(*it) == tribe::murloc) {
									other_it->stats.debuff_attack(other_it->golden ? 2 : 1);
								}
								break;
							case id::scavenging_hyena:
								if (&other_wb == &wb && get_tribe(*it) == tribe::beast) {
									bool const golden = other_it->golden;
									other_it->stats.buff_attack(golden ? 4 : 2);
									other_it->stats.buff_health(golden ? 2 : 1);
								}
								break;
							case id::baron_rivendare:
								//! @todo Trigger additional deathrattles.
								break;
							case id::junkbot:
								if (&other_wb == &wb && get_tribe(*it) == tribe::mech) {
									int const buff = other_it->golden ? 4 : 2;
									other_it->stats.buff_attack(buff);
									other_it->stats.buff_health(buff);
								}
								break;
							default:
								// No on-other-death effects.
								break;
						}
					}
				}
				{ // Trigger this minion's deathrattles.
					// Determine number of triggers based on presence of allied Baron Rivendares.
					int const dr_count = std::reduce(wb.begin(), wb.end(), 1, [it](int acc, minion const& m) {
						if (&m == &*it || m.id != id::baron_rivendare) { return acc; };
						return std::max(acc, m.golden ? 3 : 2);
					});
					for (int i = 0; i < dr_count; ++i) {
						trigger_dr(it);
					}
				}
				// Remove any aura buffs this minion was giving.
				switch (it->id) {
					case id::murloc_warleader:
						for (auto ally = wb.begin(); ally != wb.end(); ++ally) {
							if (ally != it && get_tribe(*ally) == tribe::murloc) { ally->stats.debuff_attack(2); }
						}
						break;
					case id::siegebreaker:
						for (auto ally = wb.begin(); ally != wb.end(); ++ally) {
							if (ally != it && get_tribe(*ally) == tribe::demon) { ally->stats.debuff_attack(1); }
						}
						break;
					case id::malganis:
						for (auto ally = wb.begin(); ally != wb.end(); ++ally) {
							if (ally != it && get_tribe(*ally) == tribe::demon) {
								ally->stats.debuff_attack(2);
								ally->stats.debuff_health(2);
							}
						}
						break;
					default:
						// No aura.
						break;
				}
				// Trigger reborn, if present.
				if (it->reborn) {
					summon(wb, std::next(it), create(it->id, it->golden).with_reborn(false).with_health(1), wb, {});
				}
				// Mark as dead.
				it->stats.make_dead();
			}
		}
		if (repeat) { resolve_deaths(); }
	}

	auto simulate(board const& board, int n_trials, bool trace) -> combat_results {
		std::vector<std::future<combat_results>> future_results;
		// Determine number of async tasks to create.
		int const n_tasks = std::min(n_trials, static_cast<int>(std::thread::hardware_concurrency()));
		// Create tasks.
		for (int task_idx = 0; task_idx < n_tasks; ++task_idx) {
			future_results.emplace_back(std::async([=, &board] {
				combat_results results;
				// When tasks cannot be divided evenly, front-load the leftover tasks.
				int const n_local_trials = n_trials / n_tasks + (task_idx < n_trials % n_tasks ? 1 : 0);
				for (int i = 0; i < n_local_trials; ++i) {
					results += simulation{board, trace}.results();
				}
				return results;
			}));
		}
		// Gather results.
		auto results = std::reduce( //
			future_results.begin(),
			future_results.end(),
			combat_results{},
			[](combat_results const& acc, std::future<combat_results>& fut_results) { return acc + fut_results.get(); });
		return results;
	}

	auto optimize_ally_order(board const& actual_board, int n_trials_per_alternative, goal goal) -> board {
		using namespace ranges;
		// A minion along with its original index in its warband.
		using ordered_minion = std::pair<minion, std::size_t>;
		// Compute results of original warband.
		auto actual_results = simulate(actual_board, n_trials_per_alternative);
		// Create (ally, original index) list.
		std::vector<ordered_minion> ordered_allies = //
			views::zip_with( //
				[](minion const& m, int i) {
					return ordered_minion{m, i};
				},
				actual_board[0],
				views::iota(std::size_t{0})) //
			| to<std::vector>();
		auto score_results = [goal](combat_results const& results) {
			switch (goal) {
				case goal::max_wins:
					return results.wins;
					break;
				case goal::min_losses:
					return -results.losses;
					break;
				case goal::max_net_wins:
					return results.wins - results.losses;
					break;
				case goal::max_damage_given:
					return results.allied_score;
					break;
				case goal::min_damage_taken:
					return results.enemy_score;
					break;
				case goal::max_net_damage:
					return results.net_score();
					break;
				default:
					// Unreachable.
					__assume(false);
			}
		};
		// Keep track of best board/results/score.
		auto best_board = actual_board;
		auto best_results = actual_results;
		auto best_score = score_results(actual_results);
		// Iterate over every board permutation to find the best one.
		do {
			board candidate_board{//
				views::transform(ordered_allies, &ordered_minion::first) | to<std::list>,
				actual_board[1]};
			auto const candidate_results = simulate(candidate_board, n_trials_per_alternative);
			auto const candidate_score = score_results(candidate_results);
			if (candidate_score > best_score) {
				best_board = candidate_board;
				best_results = candidate_results;
				best_score = candidate_score;
			}
		} while (next_permutation(ordered_allies, {}, &ordered_minion::second));
		return best_board;
	}
}
