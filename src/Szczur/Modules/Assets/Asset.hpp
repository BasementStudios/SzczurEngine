#pragma once

#include <memory>

#include "AssetTraits.hpp"

namespace rat
{

template<typename T>
class Asset
{
public:

	using Value_t  = T;
	using Holder_t = std::unique_ptr<Value_t>;
	using Size_t   = size_t;

private:

	Holder_t _holder;
	Size_t _refCount;

public:

	template<typename... Us>
	Asset(Us&&... args);

	Asset(const Asset&) = delete;

	Asset& operator = (const Asset&) = delete;

	Asset(Asset&&) = default;

	Asset& operator = (Asset&&) = default;

	template<typename... Us>
	bool load(Us&&... args);

	bool unload();

	Value_t* getPtr();
	const Value_t* getPtr() const;

	Value_t& get();
	const Value_t& get() const;

};

}

#include "Asset.tpp"
