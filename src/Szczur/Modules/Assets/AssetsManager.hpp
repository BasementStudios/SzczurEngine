#pragma once

#include <boost/container/flat_map.hpp>

#include "Asset.hpp"
#include "FallbackAsset.hpp"
#include "Szczur/Utility/Hash.hpp"

#include "Szczur/Utility/CompilerPortability.hpp"

namespace rat
{

template <typename... Ts>
class AssetsManager
{
public:

	using Key_t           = Hash64_t;
	template <typename U>
	using Container_t     = boost::container::flat_map<Key_t, Asset<U>>;
	template <typename U>
	using Iterator_t      = typename Container_t<U>::iterator;
	template <typename U>
	using ConstIterator_t = typename Container_t<U>::const_iterator;
	template <typename U>
	using Held_t          = std::tuple<FallbackAsset<U>, Container_t<U>>;
	using Holder_t        = std::tuple<Held_t<Ts>...>;

private:

	Holder_t _holder;

public:

	AssetsManager() = default;

	AssetsManager(const AssetsManager&) = delete;

	AssetsManager& operator = (const AssetsManager&) = delete;

	AssetsManager(AssetsManager&&) = default;

	AssetsManager& operator = (AssetsManager&&) = default;

	template <typename U>
	U& load(const std::string& path);

	template <typename U>
	bool unload(const std::string& path);

	template <typename U>
	bool isLoaded(const std::string& path) const;

	template <typename U>
	U* getPtr(const std::string& path);
	template <typename U>
	const U* getPtr(const std::string& path) const;

	template <typename U>
	U& getRef(const std::string& path);
	template <typename U>
	const U& getRef(const std::string& path) const;

private:

	Key_t _obtainKey(const std::string& path) const;

	template <typename U>
	Container_t<U>& _getContainer();

	template <typename U>
	const Container_t<U>& _getContainer() const;

	template <typename U>
	Iterator_t<U> _find(const Key_t& _Key);

	template <typename U>
	ConstIterator_t<U> _find(const Key_t& _Key) const;

};

}

#include "AssetsManager.tpp"
