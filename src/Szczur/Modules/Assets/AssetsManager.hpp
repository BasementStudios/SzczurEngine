#pragma once

#include <boost/container/flat_map.hpp>

#include "Asset.hpp"
#include "Szczur/Utility/Hash.hpp"

namespace rat
{

template<typename... TTypes>
class AssetsManager
{
public:

	using Key_t           = Hash64_t;
	template <typename TType>
	using Container_t     = boost::container::flat_map<Key_t, Asset<TType>>;
	template <typename TType>
	using Iterator_t      = typename Container_t<TType>::iterator;
	template <typename TType>
	using ConstIterator_t = typename Container_t<TType>::const_iterator;
	using Holder_t        = std::tuple<Container_t<TTypes>...>;

private:

	Holder_t _holder;

public:

	AssetsManager() = default;

	AssetsManager(const AssetsManager&) = delete;

	AssetsManager& operator = (const AssetsManager&) = delete;

	AssetsManager(AssetsManager&&) = default;

	AssetsManager& operator = (AssetsManager&&) = default;

	template<typename TType>
	bool load(const std::string& path);

	template<typename TType>
	bool unload(const std::string& path);

	template<typename TType>
	bool isLoaded(const std::string& path) const;

	template<typename TType>
	TType* getPtr(const std::string& path);
	template<typename TType>
	const TType* getPtr(const std::string& path) const;

	template<typename TType>
	TType& get(const std::string& path);
	template<typename TType>
	const TType& get(const std::string& path) const;

private:

	Key_t _getKeyFromPath(const std::string& path) const;

	template<typename TType>
	Container_t<TType>& _getContainer();

	template<typename TType>
	const Container_t<TType>& _getContainer() const;

	template<typename TType>
	Iterator_t<TType> _find(const Key_t& _Key);

	template<typename TType>
	ConstIterator_t<TType> _find(const Key_t& _Key) const;

};

}

#include "AssetsManager.tpp"
