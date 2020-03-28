#include "board.hpp"

#include <fmt/format.h>

namespace hsbg {
	auto print(board const& board) -> void {
		print(board[0]);
		fmt::print("\n - VS -\n");
		print(board[1]);
		fmt::print("\n");
	}
}
