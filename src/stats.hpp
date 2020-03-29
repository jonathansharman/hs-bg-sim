#pragma once

#include <algorithm>

namespace hsbg {
	struct stats {
		stats(int attack, int health);

		auto attack() const -> int;
		auto health() const -> int;
		auto max_health() const -> int;

		auto set_attack(int attack) -> void;
		auto buff_attack(int amount) -> void;
		auto debuff_attack(int amount) -> void;

		auto set_health(int health) -> void;
		auto lose_health(int amount) -> void;
		auto restore_health(int amount) -> void;
		auto buff_health(int amount) -> void;
		auto debuff_health(int amount) -> void;

	private:
		int _attack;
		int _health;
		int _max_health;
	};
}
