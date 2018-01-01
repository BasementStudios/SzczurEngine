#pragma once

/** @file AssetsManager.hpp
 ** @description Header file with Assets Manager class.
 ** @author Patryk (Stritch)
 ** @auhtor Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>

#include <boost/container/flat_map.hpp>

#include "Asset.hpp"
#include "Szczur/Utility/Hash.hpp"

namespace rat
{

template<typename... TTypes>
class AssetsManager
{
	/* Types */
public:
	using Key_t           = Hash64_t;
	template <typename TType>
	using Container_t     = boost::container::flat_map<Key_t, Asset<TType>>;
	template <typename TType>
	using Iterator_t      = typename Container_t<TType>::iterator;
	template <typename TType>
	using ConstIterator_t = typename Container_t<TType>::const_iterator;
	using Holder_t        = std::tuple<Container_t<TTypes>...>;



	/* Variables */
private:
	Holder_t _holder;



	/* Operators */
public:
	AssetsManager() = default;
	
	// Disable copy constructors and operators
	AssetsManager(const AssetsManager&) = delete;
	AssetsManager& operator = (const AssetsManager&) = delete;

	AssetsManager(AssetsManager&&) = default;
	AssetsManager& operator = (AssetsManager&&) = default;


	
	/* Methods */
public:
	/** @method load
	 ** @description Loads and returns reference to the loaded resource.
	 ** @template TType - return and resource type.
	 ** @argument path - relative path to the resource file.
	 ** @returns reference to the loaded resource.
	 ** @throws AssetNotFoundException
	 **/
	template<typename TType>
	TType& load(const std::string& path);

	/** @method unload
	 ** @description Unloads the resource.
	 ** @template TType - return and resource type.
	 ** @argument path - relative path to the resource file.
	 ** @argument key - key needed to obtain the resource.
	 **/
	template<typename TType>
	inline void unload(const std::string& path) 
		{ unload(getKey(path)); }
	template<typename TType>
	void unload(const Key_t& key);

	/** @method get
	 ** @description Get the resource without changing references counter. 
	 ** @returns TType& - reference to the resource. 
	 ** @throws AssetNotFoundException
	 **/
	template<typename TType>
	inline TType& get(const std::string& path)
		{ return get(getKey(path)); }
	template<typename TType>
	TType& get(const Key_t& key);
	template<typename TType>
	inline const TType& get(const std::string& path) const
		{ return get(getKey(path)); }
	template<typename TType>
	const TType& get(const Key_t& key) const;

	/** @method find
	 ** @description Find the resource without changing references counter. 
	 ** @returns TType* - pointer to the resource. nullptr - if not found.
	 **/
	template<typename TType>
	inline TType* find(const std::string& path)
		{ return find(getKey(path)); }
	template<typename TType>
	TType* find(const Key_t& key);
	template<typename TType>
	inline const TType* find(const std::string& path) const
		{ return find(getKey(path)); }
	template<typename TType>
	const TType* find(const Key_t& key) const;

	/** @method referenceCount
	 ** @description Count the references from loading/unloading the resource.
	 **/
	template<typename TType>
	inline std::size_t referenceCount(const std::string& path) const
		{ return referenceCount(getKey(path)); }
	template<typename TType>
	std::size_t referenceCount(const Key_t& key) const;

	/** @method isLoaded
	 ** @description Checks if the resource is loaded. 
	 **/
	template<typename TType>
	inline bool isLoaded(const std::string& path) const
		{ return isLoaded(getKey(path)); }
	template<typename TType>
	bool isLoaded(const Key_t& key) const;

	/** @method getKey
	 ** @description Returns key to the resource by path.
	 **/
	Key_t getKey(const std::string& path) const;
	
private:
	/// Gets container
	template<typename TType>
	inline Container_t<TType>& _getContainer()
		{ return std::get<Container_t<TType>>(_holder); }
	template<typename TType>
	inline const Container_t<TType>& _getContainer() const
		{ return std::get<Container_t<TType>>(_holder); }

	/// Finds by the key 
	template<typename TType>
	inline Iterator_t<TType> _find(const Key_t& _Key)
		{ return _getContainer<TType>().find(_Key); }
	template<typename TType>
	inline ConstIterator_t<TType> _find(const Key_t& _Key) const
		{ return _getContainer<TType>().find(_Key); }
};

}

#include "AssetsManager.tpp"
