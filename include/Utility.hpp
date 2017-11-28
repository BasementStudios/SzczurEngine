#pragma once

#include <utility>

namespace rat
{
	template<typename T>
	inline constexpr auto enum_v(T enum_var) {
		return static_cast<std::underlying_type_t<T>>(enum_var);
	}

	template<typename T, size_t U>
	constexpr unsigned int elements_c(std::array<T, U> container) {
		unsigned int result = 0;
		for (auto& i : container) {
			result += i;
		}
		return result;
	}
}