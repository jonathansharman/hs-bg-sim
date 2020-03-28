#pragma once

#include "ids.hpp"

namespace hsbg {
	enum class tier : int { one = 1, two, three, four, five, six };

	struct minion;

	auto get_tier(id const& id) -> tier;
	auto get_tier(minion const& id) -> tier;
}
