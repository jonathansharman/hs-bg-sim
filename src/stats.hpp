#pragma once

#include <algorithm>

namespace hsbg {
	struct stats {
		stats(int attack, int health);

		auto operator==(stats const&) const -> bool = default;

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

		auto alive() const -> bool;
		auto dying() const -> bool;
		auto dead() const -> bool;

		auto make_dying() -> void;
		auto make_dead() -> void;

		auto poison() -> void;
		auto poisoned() -> bool;

	private:
		enum class liveness { alive, dying, dead };

		int _attack;
		int _health;
		int _max_health;

		liveness _liveness = liveness::alive;
		bool _poisoned = false;

		/// Marks alive if not poisoned and health is positive. Should be called whenever health is restored.
		auto maybe_resurrect() -> void;
	};
}
