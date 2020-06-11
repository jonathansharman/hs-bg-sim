#pragma once

#include "stats.hpp"

namespace hsbg {
	stats::stats(int attack, int health) : _attack{attack}, _health{health}, _max_health{health} {}
	auto stats::attack() const -> int {
		return _attack;
	}
	auto stats::health() const -> int {
		return _health;
	}
	auto stats::max_health() const -> int {
		return _max_health;
	}

	auto stats::set_health(int health) -> void {
		_max_health = std::max(0, health);
		_health = _max_health;
		if (alive() && _health <= 0) {
			_liveness = liveness::marked_for_death;
		} else {
			maybe_resurrect();
		}
	}
	auto stats::set_attack(int attack) -> void {
		_attack = std::max(0, attack);
	}
	auto stats::buff_attack(int amount) -> void {
		_attack += amount;
	}
	auto stats::debuff_attack(int amount) -> void {
		_attack = std::max(0, _attack - amount);
	}

	auto stats::lose_health(int amount) -> lose_health_result {
		_health -= amount;
		if (alive() && _health <= 0) { _liveness = liveness::marked_for_death; }
		if (_health > 0) {
			return lose_health_result::survived;
		} else if (_health < 0) {
			return lose_health_result::overkilled;
		} else {
			return lose_health_result::killed;
		}
	}
	auto stats::restore_health(int amount) -> void {
		_health = std::min(_max_health, _health + amount);
		maybe_resurrect();
	}
	auto stats::buff_health(int amount) -> void {
		_max_health += amount;
		_health += amount;
		maybe_resurrect();
	}
	auto stats::debuff_health(int amount) -> void {
		_max_health -= amount;
		_health = std::min(_health, _max_health);
	}

	auto stats::alive() const -> bool {
		return _liveness == liveness::alive;
	}
	auto stats::marked_for_death() const -> bool {
		return _liveness == liveness::marked_for_death;
	}
	auto stats::will_trigger_dr() const -> bool {
		return _liveness == liveness::will_trigger_dr;
	}
	auto stats::dead() const -> bool {
		return _liveness == liveness::dead;
	}

	auto stats::mark_will_trigger_dr() -> void {
		_liveness = liveness::will_trigger_dr;
	}
	auto stats::mark_dead() -> void {
		_liveness = liveness::dead;
	}

	auto stats::poison() -> void {
		_poisoned = true;
		if (alive()) { _liveness = liveness::marked_for_death; }
	}
	auto stats::poisoned() -> bool {
		return _poisoned;
	}

	auto stats::maybe_resurrect() -> void {
		if (marked_for_death() && _health > 0 && !_poisoned) { _liveness = liveness::alive; }
	}
}
