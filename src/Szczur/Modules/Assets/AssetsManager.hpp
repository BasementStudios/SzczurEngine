#pragma once

#include <boost/container/flat_map.hpp>

#include "Asset.hpp"
#include "Szczur/Utility/Hash.hpp"

namespace rat
{

template<typename... Ts>
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
	using Holder_t        = std::tuple<Container_t<Ts>...>;

private:

	Holder_t _holder;

public:

	AssetsManager() = default;

	AssetsManager(const AssetsManager&) = delete;

	AssetsManager& operator = (const AssetsManager&) = delete;

	AssetsManager(AssetsManager&&) = default;

	AssetsManager& operator = (AssetsManager&&) = default;

	template<typename U>
	bool load(const std::string& _Path);

	template<typename U>
	bool unload(const std::string& _Path);

	template<typename U>
	bool isLoaded(const std::string& _Path) const;

	template<typename U>
	U* getPtr(const std::string& path);
	template<typename U>
	const U* getPtr(const std::string& path) const;

	template<typename U>
	U& get(const std::string& path);
	template<typename U>
	const U& get(const std::string& path) const;

private:

	Key_t _getKeyFromPath(const std::string& _Path) const;

	template<typename U>
	Container_t<U>& _getContainer();

	template<typename U>
	const Container_t<U>& _getContainer() const;

	template<typename U>
	Iterator_t<U> _find(const Key_t& _Key);

	template<typename U>
	ConstIterator_t<U> _find(const Key_t& _Key) const;

};

}

#include "AssetsManager.tpp"
