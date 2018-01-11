#pragma once

#include <cstddef>

#include "TotalOffsetOf.hpp"

namespace rat
{

template <typename... Ts>
class VariadicPackInfo
{
public:

	template <typename U>
	static constexpr size_t offsetOf()
	{
		return TotalOffsetOf_v<0, U, Ts...>;
	}

	static constexpr size_t typesCount()
	{
		return sizeof...(Ts);
	}

};

}
