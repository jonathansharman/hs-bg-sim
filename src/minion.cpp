#include "minion.hpp"

#include "names.hpp"

#include <fmt/format.h>

namespace hsbg {
	minion::minion(hsbg::id id, hsbg::stats stats) : id{id}, stats{stats} {}

	auto minion::alive() const->bool {
		return _liveness == liveness::alive;
	}
	auto minion::dying() const -> bool {
		return _liveness == liveness::dying;
	}
	auto minion::dead() const -> bool {
		return _liveness == liveness::dead;
	}

	auto minion::make_alive() -> void {
		_liveness = liveness::alive;
	}
	auto minion::make_dying() -> void {
		_liveness = liveness::dying;
	}
	auto minion::make_dead() -> void {
		_liveness = liveness::dead;
	}

	auto minion::with_stats(hsbg::stats s) -> minion& {
		stats = s;
		return *this;
	}
	auto minion::with_attack(int attack) -> minion& {
		stats.set_attack(attack);
		return *this;
	}
	auto minion::with_health(int health) -> minion& {
		stats.set_health(health);
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

	auto operator<<(std::ostream& out, minion const& m) -> std::ostream& {
		if (m.golden) { out << "golden "; }
		out << fmt::format("{}/{} {}", m.stats.attack(), m.stats.health(), get_name(m));
		if (m.taunt) { out << ", taunt"; }
		if (m.ds) { out << ", divine shield"; }
		if (m.reborn) { out << ", reborn"; }
		if (m.poisonous) { out << ", poisonous"; }
		if (m.attack_count == 2) { out << ", windfury"; }
		return out;
	}

	auto create(id id, bool golden) -> minion {
		// This works because id is backed by a zero-indexed int. This saves having to construct an actual map.
		minion result = all_minions[static_cast<int>(id)];
		if (golden) {
			result.golden = true;
			result.stats.set_attack(2 * result.stats.attack());
			result.stats.set_health(2 * result.stats.health());
			if (id == id::zapp_slywick) {
				// Slywick gains mega-windfury when golden.
				result.attack_count = 4;
			}
		}
		return result;
	}
}
