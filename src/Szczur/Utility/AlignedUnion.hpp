#pragma once

#include <cstddef>
#include <type_traits>

#include "TotalOffsetOf.hpp"

namespace rat
{

template <typename... Ts>
class AlignedUnion
{
public:

	using Byte_t         = std::byte;
	using Pointer_t      = Byte_t*;
	using ConstPointer_t = const Byte_t*;
	using Storage_t      = std::aligned_union_t<(... + sizeof(Ts)), Ts...>;

	AlignedUnion()
	{
		(_create<Ts>(), ...);
	}

	~AlignedUnion()
	{
		(_destroy<Ts>(), ...);;
	}

	template <typename U>
	U* getMemoryOf()
	{
		return reinterpret_cast<U*>(_getStoragePtr() + TotalOffsetOf_v<0u, U, Ts...>);
	}

	template <typename U>
	const U* getMemoryOf() const
	{
		return reinterpret_cast<const U*>(_getStoragePtr() + TotalOffsetOf_v<0u, U, Ts...>);
	}

private:

	Pointer_t _getStoragePtr()
	{
		return reinterpret_cast<Pointer_t>(&_storage);
	}

	ConstPointer_t _getStoragePtr() const
	{
		return reinterpret_cast<ConstPointer_t>(&_storage);
	}

	template <typename U>
	void _create()
	{
		new (getMemoryOf<U>()) U;
	}

	template <typename U>
	void _destroy()
	{
		// @todo Y U N workin' properly ;-;
		// delete getMemoryOf<U>();
	}

	Storage_t _storage;

};

}
