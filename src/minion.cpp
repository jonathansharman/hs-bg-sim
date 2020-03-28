#include "minion.hpp"

#include "names.hpp"

#include <fmt/format.h>

namespace hsbg {
	auto minion::with_stats(int attack, int health) -> minion& {
		stats = {attack, health, health};
		return *this;
	}
	auto minion::with_attack(int attack) -> minion& {
		stats.attack = attack;
		return *this;
	}
	auto minion::with_health(int health) -> minion& {
		stats.health = health;
		stats.max_health = health;
		return *this;
	}
	auto minion::with_taunt() -> minion& {
		taunt = true;
		return *this;
	}
	auto minion::with_ds() -> minion& {
		ds = true;
		return *this;
	}
	auto minion::with_reborn(bool value) -> minion& {
		reborn = value;
		return *this;
	}
	auto minion::with_poisonous() -> minion& {
		poisonous = true;
		return *this;
	}
	auto minion::with_windfury() -> minion& {
		attack_count = 2;
		return *this;
	}
	auto minion::with_megawindfury() -> minion& {
		attack_count = 4;
		return *this;
	}
	auto minion::with_dr(dr dr) -> minion& {
		drs.push_back(dr);
		return *this;
	}
	auto minion::with_on_attack(on_attack on_attack) -> minion& {
		on_attacks.push_back(on_attack);
		return *this;
	}
	auto minion::with_on_friendly_death(on_friendly_death on_friendly_death) -> minion& {
		on_friendly_deaths.push_back(on_friendly_death);
		return *this;
	}
	auto minion::with_on_friendly_kill(on_friendly_kill on_friendly_kill) -> minion& {
		on_friendly_kills.push_back(on_friendly_kill);
		return *this;
	}
	auto minion::with_on_damage(on_damage on_damage) -> minion& {
		on_damages.push_back(on_damage);
		return *this;
	}

	auto print(minion const& m) -> void {
		if (m.golden) { fmt::print("golden "); }
		fmt::print("{}/{} {}", m.stats.attack, m.stats.health, get_name(m));
		if (m.taunt) { fmt::print(", taunt"); }
		if (m.ds) { fmt::print(", divine shield"); }
		if (m.reborn) { fmt::print(", reborn"); }
		if (m.poisonous) { fmt::print(", poisonous"); }
		if (m.attack_count == 2) { fmt::print(", windfury"); }
	}

	auto create(id id, bool golden) -> minion {
		// This works because id is backed by a zero-indexed int. This saves having to construct an actual map.
		minion result = all_minions[static_cast<int>(id)];
		if (golden) {
			result.golden = true;
			result.stats.attack *= 2;
			result.stats.health *= 2;
			if (id == id::zapp_slywick) {
				// Slywick gains mega-windfury when golden.
				result.attack_count = 4;
			}
		}
		return result;
	}
}
