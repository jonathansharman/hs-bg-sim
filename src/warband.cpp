#include "warband.hpp"

#include "minion.hpp"
#include "random.hpp"

#include <fmt/format.h>

namespace hsbg {
	auto operator<<(std::ostream& out, warband const& warband) -> std::ostream& {
		if (warband.empty()) {
			out << "<empty>";
			return out;
		}
		out << warband.front();
		for (auto it = std::next(warband.begin()); it != warband.end(); ++it) {
			out << "; " << *it;
		}
		return out;
	};

	auto random_warband() -> warband {
		warband result;
		int const count = rand_int(1, 7);
		for (int i = 0; i < count; ++i) {
			id const random_id = static_cast<id>(rand_int(0, static_cast<int>(id::COUNT) - 1));
			result.push_back(create(random_id));
		}
		return result;
	};
}
