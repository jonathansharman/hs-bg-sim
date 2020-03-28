#include "simulation.hpp"

#include "random.hpp"
#include "tiers.hpp"
#include "tribes.hpp"

#include <fmt/format.h>

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
			return choose(filtered_its(wb, [](minion const& m) { return m.liveness == liveness::alive; }));
		}
	}

	simulation::simulation(board const& board) : _board{board} {}

	auto simulation::run(bool trace) -> int {
		if (trace) { print(_board); }

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
					if (attacker->liveness == liveness::alive && attacker->stats.attack > 0) {
						_current_attackers[active_wb_idx] = std::next(attacker);
						return attacker;
					}
				}
				// No viable attackers found towards the right. Wrap back around.
				for (wb_it attacker = _board[active_wb_idx].begin(); attacker != _current_attackers[active_wb_idx]; ++attacker) {
					if (attacker->liveness == liveness::alive && attacker->stats.attack > 0) {
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
				for (int i = 0; i < attacker->attack_count && attacker->liveness == liveness::alive; ++i) {
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
						if (it->liveness == liveness::dead) {
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

			if (trace) {
				fmt::print("==============================\n");
				print(_board);
			}

			// End combat if both warbands are unable to attack in a row.
			if (current_warband_passed && previous_warband_passed) {
				break;
			} else {
				previous_warband_passed = current_warband_passed;
			}
		}
		return score();
	}

	auto simulation::simulate(int n_trials, bool trace) -> void {
		int wins = 0;
		int draws = 0;
		int losses = 0;
		int total_score = 0;
		// If not printing a full trace, at least print the starting board.
		if (!trace) { print(_board); }
		for (int i = 0; i < n_trials; ++i) {
			auto copy = *this;
			int const score = copy.run(trace);
			if (score > 0) {
				++wins;
			} else if (score < 0) {
				++losses;
			} else {
				++draws;
			}
			total_score += score;
		}
		fmt::print("==============================\n");
		fmt::print("After {} simulation{}...\n", n_trials, n_trials == 1 ? "" : "s");
		fmt::print("  wins: {} ({}%), draws: {} ({}%), losses: {} ({}%)\n",
			wins,
			100.0 * wins / n_trials,
			draws,
			100.0 * draws / n_trials,
			losses,
			100.0 * losses / n_trials);
		fmt::print("  mean score: {}\n\n", 1.0 * total_score / n_trials);
	}

	auto simulation::score() const -> int {
		int const warband0_score = std::reduce(_board[0].begin(), _board[0].end(), 0, [](int sum, minion const& m) {
			return sum + static_cast<int>(get_tier(m.id));
		});
		int const warband1_score = std::reduce(_board[1].begin(), _board[1].end(), 0, [](int sum, minion const& m) {
			return sum + static_cast<int>(get_tier(m.id));
		});
		return warband0_score - warband1_score;
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
		std::unordered_set<minion*> khadgars) -> wb_it //
	{
		auto const live_count = std::count_if(
			allies.begin(), allies.end(), [](minion const& m) { return m.liveness == liveness::alive; });
		if (live_count < 7) {
			// Apply auras.
			for (auto const& ally : allies) {
				switch (ally.id) {
					case id::murloc_warleader:
						if (get_tribe(summoned) == tribe::murloc) { summoned.stats.attack += 2; }
						break;
					case id::siegebreaker:
						if (get_tribe(summoned) == tribe::demon) { summoned.stats.attack += 1; }
						break;
					case id::malganis:
						if (get_tribe(summoned) == tribe::demon) {
							summoned.stats.attack += 2;
							summoned.stats.health += 2;
						}
						break;
					default:
						// Ally has no aura.
						break;
				}
			}

			// Apply on-summon effects.
			for (auto& wb : _board) {
				bool const allied = &wb == &allies;
				for (auto it = wb.begin(); it != wb.end(); ++it) {
					switch (it->id) {
						case id::old_murk_eye:
							// Gains 1 (golden: 2) attack when a new murloc appears anywhere.
							if (get_tribe(summoned) == tribe::murloc) { it->stats.attack += it->golden ? 2 : 1; }
							break;
						case id::cobalt_guardian:
							// Gains divine shield when friendly mechs are summoned.
							if (allied && get_tribe(summoned) == tribe::mech) { it->ds = true; }
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
							if (allied && get_tribe(summoned) == tribe::beast) {
								summoned.stats.attack += it->golden ? 6 : 3;
							}
							break;
						case id::mama_bear:
							// Gives friendly summoned beasts +4/+4 (golden: +8/+8).
							if (allied && get_tribe(summoned) == tribe::beast) {
								int const buff = it->golden ? 8 : 4;
								summoned.stats.attack += buff;
								summoned.stats.health += buff;
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
					if (recipient) { (*recipient)->stats.attack += dying_it->stats.attack; }
				}
				break;
			case id::mecharoo:
				summon(allies, std::next(dying_it), create(id::jo_e_bot, dying_it->golden), allies, {});
				break;
			case id::selfless_hero: {
				// Give divine shield to one (golden: two) random minions without divine shield already.
				for (int i = 0; i < (dying_it->golden ? 2 : 1); ++i) {
					auto recipient = choose(
						filtered_its(allies, [](minion const& m) { return m.liveness == liveness::alive && !m.ds; }));
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
					if (auto target = choose_alive(get_enemies(allies))) {
						take_damage(*target, 4, dying_it->poisonous);
					}
				}
				break;
			case id::kindly_grandmother:
				summon(allies, std::next(dying_it), create(id::big_bad_wolf, dying_it->golden), allies, {});
				break;
			case id::rat_pack:
				// Summon rats (golden: golden rats) equal to the rat pack's attack.
				summon_n(dying_it->stats.attack, allies, std::next(dying_it), create(id::rat, dying_it->golden), allies);
				break;
			case id::spawn_of_nzoth:
				// Give all other minions +1/+1 (golden: +2/+2).
				for (auto& ally : allies) {
					int const buff = dying_it->golden ? 2 : 1;
					if (&ally != &*dying_it) {
						ally.stats.attack += buff;
						ally.stats.health += buff;
					}
				}
				break;
			case id::unstable_ghoul:
				// Deal 1 (golden: 2) damage to all other minions.
				for (auto& warband : _board) {
					for (auto it = warband.begin(); it != warband.end(); ++it) {
						if (&*it != &*dying_it) { take_damage(it, dying_it->golden ? 2 : 1, dying_it->poisonous); }
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
						ally->stats.attack += buff;
						ally->stats.health += buff;
					}
				}
				break;
			case id::king_bagurgle:
				// Give allied murlocs +2/+2 (golden: +4/+4).
				for (auto ally = allies.begin(); ally != allies.end(); ++ally) {
					int const buff = dying_it->golden ? 4 : 2;
					if (ally != dying_it && get_tribe(*ally) == tribe::murloc) {
						ally->stats.attack += buff;
						ally->stats.health += buff;
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
			int min_health = enemies.front().stats.health;
			targets.push_back(enemies.begin());
			for (auto it = std::next(enemies.begin()); it != enemies.end(); ++it) {
				int const health = it->stats.health;
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

	auto simulation::take_damage(wb_it target, int damage, bool poisonous) -> void {
		if (damage == 0) { return; }
		if (target->ds) {
			target->ds = false;
			// Apply loss-of-divine-shield effects.
			for (auto& ally : get_allies(target)) {
				if (&ally != &*target) {
					switch (ally.id) {
						case id::bolvar_fireblood:
							ally.stats.attack += ally.golden ? 4 : 2;
							break;
						case id::drakonid_enforcer: {
							int const buff = ally.golden ? 4 : 2;
							ally.stats.attack += buff;
							ally.stats.health += buff;
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
		} else if (target->liveness == liveness::alive) {
			target->stats.health -= damage;
			if (poisonous) {
				target->poisoned = true;
				target->liveness = liveness::dying;
			} else if (target->stats.health <= 0) {
				target->liveness = liveness::dying;
			}
		}
	}

	auto simulation::attack(wb_it attacker, wb_it primary_target) -> void {
		auto& enemies = get_allies(primary_target);
		if (attacker->id == id::glyph_guardian) {
			// Apply Glyph Guardian effect.
			attacker->stats.attack *= attacker->golden ? 3 : 2;
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
			take_damage(target, attacker->stats.attack, attacker->poisonous);
		}
		// Hit attacker.
		take_damage(attacker, primary_target->stats.attack, primary_target->poisonous);
	}

	auto simulation::resolve_deaths() -> void {
		bool repeat = false;
		for (auto& wb : _board) {
			/// @todo How is it determined which player's minion deaths resolve first?
			for (auto it = wb.begin(); it != wb.end(); ++it) {
				// Only interested in minions marked as dying.
				if (it->liveness != liveness::dying) { continue; }
				// Check if the minion is actually dead.
				if (it->stats.health <= 0 || it->poisoned) {
					// A minion has died. Will need to perform death resolution again.
					repeat = true;

					trigger_dr(it);
					// Remove any aura buffs this minion was giving.
					switch (it->id) {
						case id::murloc_warleader:
							for (auto ally = wb.begin(); ally != wb.end(); ++ally) {
								if (ally != it && get_tribe(*ally) == tribe::murloc) {
									ally->stats.attack = std::max(0, ally->stats.attack - 2);
								}
							}
							break;
						case id::siegebreaker:
							for (auto ally = wb.begin(); ally != wb.end(); ++ally) {
								if (ally != it && get_tribe(*ally) == tribe::demon) {
									ally->stats.attack = std::max(0, ally->stats.attack - 1);
								}
							}
							break;
						case id::malganis:
							for (auto ally = wb.begin(); ally != wb.end(); ++ally) {
								if (ally != it && get_tribe(*ally) == tribe::demon) {
									ally->stats.attack = std::max(0, ally->stats.attack - 2);
									/// @todo The loss of health buffs needs to account for damage taken.
									ally->stats.health = std::max(1, ally->stats.health - 2);
								}
							}
							break;
						default:
							// No aura.
							break;
					}
					if (it->reborn) {
						// Reborn triggers after deathrattles.
						summon(wb, std::next(it), create(it->id, it->golden).with_reborn(false).with_health(1), wb, {});
					}
					// Mark as dead.
					it->liveness = liveness::dead;
				} else {
					// The minion was brought back to life by a DR health buff.
					it->liveness = liveness::alive;
				}
			}
		}
		if (repeat) { resolve_deaths(); }
	}
}
