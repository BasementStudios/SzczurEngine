#pragma once

/** @file Assets.hpp
 ** @description Main header file of the Assets module. 
 ** @module Assets
 ** @author Patryk (Stritch)
 ** @auhtor Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "Szczur/Utility/Modules.hpp"
#include "AssetsManager.hpp"

namespace rat
{

/** @class Assets
 ** @inherits Module
 ** @description Main module class.
 **/
class Assets : public Module<>
{
	using Module::Module;

	/* Types */
public:
	using Manager_t = AssetsManager<sf::Font, sf::Texture, sf::Shader, sf::Music, sf::SoundBuffer>;
	using Key_t = Manager_t::Key_t;



	/* Variable */
public:
	Manager_t _manager;



	/* Methods */
public:
	// Module initializae
	void init();

	// @info @warn Nie jestem pewny, czy to w ogólne będzie używane...
	bool loadFromJsonFile(const std::string& path);

	/** @method load
	 ** @description Loads and returns reference to the loaded resource.
	 ** @template TType - return and resource type.
	 ** @argument path - relative path to the resource file.
	 ** @argument key - key needed to obtain the resource.
	 ** @returns reference or pointer to the loaded resource.
	 ** @throws AssetNotFoundException
	 **/
	template<typename TType>
	inline TType load(const std::string& path) 			
		{ return this->_manager.load<TType>(path); }

	/** @method unload
	 ** @description Unloads the resource.
	 **/
	template<typename TType>
	inline bool unload(const std::string& path)			
		{ return this->_manager.unload<TType>(path); }
	template<typename TType>
	inline bool unload(const Key_t& key)					
		{ return this->_manager.unload<TType>(key); }

	/** @method get
	 ** @description Get the resource without changing references counter.
	 ** @returns TType& - reference to the resource. 
	 **/
	template<typename TType>
	inline TType& get(const std::string& path)
		{ return this->_manager.get<TType>(path); }
	template<typename TType>
	inline TType& get(const Key_t& key)
		{ return this->_manager.get<TType>(key); }
	template<typename TType>
	inline const TType& get(const std::string& path) const
		{ return this->_manager.get<TType>(path); }
	template<typename TType>
	inline const TType& get(const Key_t& key) const
		{ return this->_manager.get<TType>(key); }

	/** @method find
	 ** @description Find the resource without changing references counter.
	 ** @returns TType* - pointer to the resource. 
	 **/
	template<typename TType>
	inline TType* find(const std::string& path)
		{ return this->_manager.get<TType>(path); }
	template<typename TType>
	inline TType* find(const Key_t& key)
		{ return this->_manager.find<TType>(key); }
	template<typename TType>
	inline const TType* find(const std::string& path) const
		{ return this->_manager.find<TType>(path); }
	template<typename TType>
	inline const TType* find(const Key_t& key) const
		{ return this->_manager.find<TType>(key); }

	/** @method referenceCount
	 ** @description Count the references from loading/unloading the resource.
	 **/
	template<typename TType>
	inline std::size_t referenceCount(const std::string& path) const
		{ return this->_manager.referenceCount<TType>(path); }
	template<typename TType>
	inline std::size_t referenceCount(const Key_t& key) const
		{ return this->_manager.referenceCount<TType>(key); }

	/** @method isLoaded
	 ** @description Checks if the resource is loaded. 
	 **/
	template<typename TType>
	inline bool isLoaded(const std::string& path) const 
		{ return this->_manager.isLoaded<TType>(path); }
	template<typename TType>
	inline bool isLoaded(const Key_t& key) const 
		{ return this->_manager.isLoaded<TType>(key); }

	/** @method getKey
	 ** @description Returns key to the resource by path.
	 **/
	Key_t getKey(const std::string& path) const
		{ return this->_manager.getKey(path); }
};

}
