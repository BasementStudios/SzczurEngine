#pragma once

/** @file World.hpp
 ** @description Header file with main class of the World module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Debug.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "ObjectType.hpp"
#include "Map.hpp"

namespace rat {

/** @class World
 ** @description Manages objects in the game world through the maps system, including loading and saving, object types management and other.
 **/
class World : public Module<Window>
{
    using Module::Module;


	
    /* Variables */
private:
    std::vector<ObjectType*> types; // @todo , Use some better container; (std::list?)
	
	std::vector<Map>	maps;
	Map* 				currentMap;
	


	/* Operators */
public:
	// Module constructor/destructor
	World();
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

}
