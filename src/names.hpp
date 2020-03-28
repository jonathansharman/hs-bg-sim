#pragma once

#include "ids.hpp"

#include <string>

namespace hsbg {
	struct minion;

	/// Gets the human-readable name of a minion ID.
	auto get_name(id id) -> std::string;

	/// Gets the human-readable name of a minion.
	auto get_name(minion const& m) -> std::string;
}
