#pragma once

#include "AssetTraits.hpp"

namespace rat
{

template <typename T>
class Asset
{
public:

	using Value_t          = T;
	using Reference_t      = Value_t&;
	using ConstReference_t = const Value_t&;
	using Pointer_t        = Value_t*;
	using ConstPointer_t   = const Value_t*;
	using Size_t           = size_t;
	using Traits_t         = AssetTraits<Value_t>;

private:

	Pointer_t _ptr;
	Size_t _refCount;

public:

	Asset();

	Asset(const Asset&) = delete;

	Asset& operator = (const Asset&) = delete;

	Asset(Asset&& rhs) noexcept;

	Asset& operator = (Asset&& rhs) noexcept;

	~Asset();

	template <typename... Us>
	bool load(Us&&... args);

	bool unload();

	bool forceUnload();

	Pointer_t getPtr();
	ConstPointer_t getPtr() const;

	Reference_t getRef();
	ConstReference_t getRef() const;

	bool isValid() const;

	bool isLoaded() const;

private:

	void _clear();

};

}

#include "Asset.tpp"
