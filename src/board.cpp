#include "board.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace hsbg {
	auto operator<<(std::ostream& out, board const& board) -> std::ostream& {
		auto fmt_string =
			">>> {}\n"
			">> VS\n"
			">>> {}";
		out << fmt::format(fmt_string, board[0], board[1]);
		return out;
	}
}
