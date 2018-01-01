/** @file AssetsManager.tpp
 ** @description Templates implementation file of Assets Manager class.
 ** @author Patryk (Stritch)
 ** @auhtor Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>						// string, to_string

#include "AssetNotFoundException.hpp"

namespace rat
{

/// Loads and returns reference to the loaded resource.
template<typename... TTypes>
template<typename TType>
TType& AssetsManager<TTypes...>::load(const std::string& path)
{
	return *(_getContainer<TType>().try_emplace(getKey(path)).first->second.load(path));
}

/// Unloads the resource.
template<typename... TTypes>
template<typename TType>
bool AssetsManager<TTypes...>::unload(const AssetsManager<TTypes...>::Key_t& key)
{
	if (auto it = _find<TType>(key); it != _getContainer<TType>().end()) {
		if (it->second.unload()) {
			_getContainer<TType>().erase(it);
			return true;
		}
	}
	return false;
}

/// Get the resource (reference) without changing references counter. 
template<typename... TTypes>
template<typename TType>
TType& AssetsManager<TTypes...>::get(const AssetsManager<TTypes...>::Key_t& key)
{
	if (auto it = _find(key); it != _getContainer<TType>().end()) {
		return it.second.ref();
	}

	throw AssetNotFoundException(std::to_string((std::size_t)key));
	return nullptr;
}
template<typename... TTypes>
template<typename TType>
const TType& AssetsManager<TTypes...>::get(const AssetsManager<TTypes...>::Key_t& key) const
{
	if (auto it = _find(key); it != _getContainer<TType>().end()) {
		return it.second.ref();
	}

	throw AssetNotFoundException(std::to_string((std::size_t)key));
	return nullptr;
}

/// Find the resource (pointer) without changing references counter. 
template<typename... TTypes>
template<typename TType>
TType* AssetsManager<TTypes...>::find(const AssetsManager<TTypes...>::Key_t& key)
{
	if (auto it = _find(key); it != _getContainer<TType>().end()) {
		return it.second.ptr();
	}

	return nullptr;
}
template<typename... TTypes>
template<typename TType>
const TType* AssetsManager<TTypes...>::find(const AssetsManager<TTypes...>::Key_t& key) const
{
	if (auto it = _find(key); it != _getContainer<TType>().end()) {
		return it.second.ptr();
	}

	return nullptr;
}

/// Count the references from loading/unloading the resource.
template<typename... TTypes>
template<typename TType>
std::size_t AssetsManager<TTypes...>::referenceCount(const AssetsManager<TTypes...>::Key_t& key) const
{
	if (auto it = _find(key); it != _getContainer<TType>().end()) {
		return it.second.referenceCount;
	}

	return 0;
}

/// Checks if the resource is loaded. 
template<typename... TTypes>
template<typename TType>
bool AssetsManager<TTypes...>::isLoaded(const AssetsManager<TTypes...>::Key_t& key) const
{
	return _find<TType>(key) != _getContainer<TType>().end();
}

/// Gets key (hash) from the path.
template<typename... TTypes>
typename AssetsManager<TTypes...>::Key_t AssetsManager<TTypes...>::getKey(const std::string& path) const
{	// @todo , Rozwijanie  `../` `/./` itp.
	return fnv1a_64(path.begin(), path.end());
}

}
