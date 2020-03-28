#include "warband.hpp"

#include "minion.hpp"
#include "random.hpp"

#include <fmt/format.h>

namespace hsbg {
	auto print(warband const& warband) -> void {
		if (warband.empty()) {
			fmt::print("<empty>");
			return;
		}
		print(warband.front());
		for (auto it = std::next(warband.begin()); it != warband.end(); ++it) {
			fmt::print("; ");
			print(*it);
		}
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
