#pragma once

/** @file World.hpp
 ** @description Header file with main class of the World module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Szczur/Utility/Module.hpp"
#include "Szczur/Debug/Logger.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Assets/Assets.hpp"
#include "ObjectType.hpp"
#include "Map.hpp"

namespace rat {

/** @class World
 ** @description Manages objects in the game world through the maps system, including loading and saving, object types management and other.
 **/
class World : public Module<Window, Assets>
{
    using Module::Module;


	
	/* Types */
public:
	using Vecotr_t = sf::Vector2f;


	
    /* Variables */
private:
    std::vector<ObjectType*> types; // @todo , Use some better container; (std::list?)
	
	std::vector<Map>	maps;
	Map* 				currentMap;
	


	/* Operators */
public:
	// Module constructor/destructor
	template <typename ModulesTuple>
	World(ModulesTuple&& tuple);
	~World();

	// Disable coping
	World(const World&) = delete;
	World& operator = (const World&) = delete;

	// Disable moving
	World(World&&) = delete;
	World& operator = (World&&) = delete;



	/* Methods */
public:
    // Module system
	void init();
	void update(float deltaTime);
	void render();

	// @todo . load
	// @todo . save
};



// Module constructor/destructor
template <typename ModulesTuple>
inline World::World(ModulesTuple&& tuple) : Module(tuple)
{
	LOG_INFO("[World] Module initializing"); 
	this->init();
	LOG_INFO("[World] Module initialized!"); 
}
inline World::~World()
{
    LOG_INFO("[World] Module destructed"); 
}

}
