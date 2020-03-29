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

	auto stats::lose_health(int amount) -> void {
		_health -= amount;
	}
	auto stats::restore_health(int amount) -> void {
		_health = std::min(_max_health, _health + amount);
	}
	auto stats::buff_health(int amount) -> void {
		_max_health += amount;
		_health += amount;
	}
	auto stats::debuff_health(int amount) -> void {
		_max_health -= amount;
		_health = std::min(_health, _max_health);
	}
}