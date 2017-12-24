#pragma once

#include <memory>

#include "AssetTraits.hpp"

namespace rat
{

template<typename T>
class Asset
{
public:

	using Value_t           = T;
	using Reference_t       = Value_t&;
	using ConstReference_t  = const Value_t&;
	using Pointer_t         = Value_t*;
	using ConstPointer_t    = const Value_t*;
	using Size_t            = size_t;

private:

	Pointer_t _ptr;
	Size_t _refCount;

public:

	template<typename... Us>
	Asset(Us&&... args);

	Asset(const Asset&) = delete;

	Asset& operator = (const Asset&) = delete;

	Asset(Asset&&) = default;

	Asset& operator = (Asset&&) = default;

	~Asset();

	template<typename... Us>
	bool load(Us&&... args);

	template<typename... Us>
	bool unload(Us&&... args);

	Pointer_t getPtr();
	ConstPointer_t getPtr() const;

	Reference_t get();
	ConstReference_t get() const;

};

}

#include "Asset.tpp"
