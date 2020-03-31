#pragma once

#include "ids.hpp"

namespace hsbg {
	enum class tribe { none, mech, beast, demon, murloc, dragon, all };

	struct minion;

	auto get_tribe(id id) -> tribe;

	auto get_tribe(minion const& m) -> tribe;

	auto is_tribeless(id id) -> bool;
	auto is_mech(id id) -> bool;
	auto is_beast(id id) -> bool;
	auto is_demon(id id) -> bool;
	auto is_murloc(id id) -> bool;
	auto is_dragon(id id) -> bool;

	auto is_tribeless(minion const& m) -> bool;
	auto is_mech(minion const& m) -> bool;
	auto is_beast(minion const& m) -> bool;
	auto is_demon(minion const& m) -> bool;
	auto is_murloc(minion const& m) -> bool;
	auto is_dragon(minion const& m) -> bool;
}
