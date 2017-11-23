#pragma once

#include <functional>
#include <tuple>

namespace rat {
	namespace detail {
		template<size_t I, typename T, typename F>
		inline constexpr void forEach(T&& tuple, F&& function);
	}

	template<typename T, typename F>
	inline constexpr void forEach(T&& tuple, F&& function);
}

#include "ForEach.tpp"
