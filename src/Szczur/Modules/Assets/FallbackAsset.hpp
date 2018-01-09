#pragma once

#include "AssetTraits.hpp"

namespace rat
{

template <typename T>
class FallbackAsset
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

public:

	FallbackAsset();

	FallbackAsset(const FallbackAsset&) = delete;

	FallbackAsset& operator = (const FallbackAsset&) = delete;

	FallbackAsset(FallbackAsset&& rhs) noexcept;

	FallbackAsset& operator = (FallbackAsset&& rhs) noexcept;

	~FallbackAsset();

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

#include "FallbackAsset.tpp"
