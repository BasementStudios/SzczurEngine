#pragma once

#include <cstddef>

namespace rat
{

template <size_t, typename, typename...>
struct TotalOffsetOf;

template <size_t N, typename T, typename U, typename... Ts>
struct TotalOffsetOf<N, T, U, Ts...>
{
	static inline constexpr auto value = 0u + TotalOffsetOf<N + sizeof(U), T, Ts...>::value;
};

template <size_t N, typename T, typename... Ts>
struct TotalOffsetOf<N, T, T, Ts...>
{
	static inline constexpr auto value = N;
};

template <size_t N, typename T, typename... Ts>
inline constexpr size_t TotalOffsetOf_v = TotalOffsetOf<N, T, Ts...>::value;

}
