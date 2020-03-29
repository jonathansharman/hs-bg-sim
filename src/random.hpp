#pragma once

#include <random>

template <typename T>
inline auto rand_int(T start_inc, T end_inc) -> T {
	thread_local std::mt19937 prng{std::random_device{}()};
	std::uniform_int_distribution<T> dist(start_inc, end_inc);
	return dist(prng);
}

template <typename Container>
inline auto rand_element(Container&& container) {
	return container[rand_int(std::size_t{0}, std::size(container) - 1)];
}
