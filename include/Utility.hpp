#pragma once

#include <utility>

namespace rat
{
	template<typename T>
	inline constexpr auto enum_v(T enum_var) {
		return static_cast<std::underlying_type_t<T>>(enum_var);
	}
}