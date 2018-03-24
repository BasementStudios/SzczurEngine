#pragma once

#include <utility>
#include <numeric>

namespace rat
{
	template<typename T>
	inline constexpr auto enum_v(T enum_var) {
		return static_cast<std::underlying_type_t<T>>(enum_var);
	}

	template<typename T, size_t N>
	constexpr unsigned int elements_c(const std::array<T, N>& container) {
		return std::accumulate(container.begin(), container.end(), 0);
	}
}