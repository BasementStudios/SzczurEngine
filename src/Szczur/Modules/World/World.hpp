#pragma once

/** @file World.hpp
 ** @description Header file with main class of the World module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>
#include <list>

#include "Szczur/Utility/Module.hpp"
#include "Szczur/Debug/Logger.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Assets/Assets.hpp"
#include "GameObject.hpp"
#include "GameObjectType.hpp"

namespace rat {

/** @class World
 ** @description Manages objects in the game world.
 **/
class World : public Module<Window, Assets>
{
    using Module::Module;
    


    /* Types */
    using Object_t = rat::GameObject;
	
	template <typename TObjectType>
	using Container_t = std::vector<TObjectType>; // @todo . There should be custom 
	


    /* Variables */
private:
    std::vector<GameObjectType> objectTypes; // @todo , Use some better container; (std::list?)
	
	Container_t<Object_t>		sceneObjects;
	sf::RenderStates			sceneRenderStates	= sf::RenderStates::Default;
	//Container_t<Object_t>		backObjects; // @todo .
	//sf::RenderStates			backRenderStates	= sf::RenderStates::Default;
	//Container_t<Object_t>		foreObjects; // @todo .
	//sf::RenderStates			foreRenderStates	= sf::RenderStates::Default;



	/* Operators */
public:
	// Module constructor/destructor
	template <typename ModulesTuple>
	World(ModulesTuple&& tuple);
	~World();

	// Disable copy operators
	World(const World&) = delete;
	World& operator = (const World&) = delete;

	// Disable move operators
	World(World&&) = delete;
	World& operator = (World&&) = delete;



	/* Methods */
public:
    /// Module system
	void init();
	void update(float deltaTime);
	void render();

	// @todo . load
	// @todo . save
};



// Module constructor/destructor
template <typename ModulesTuple>
World::World(ModulesTuple&& tuple) :
	Module(tuple)
{
	LOG_INFO("[World] Module initializing"); 
	init();
	LOG_INFO("[World] Module initialized!"); 
}
inline World::~World()
{
    LOG_INFO("[World] Module destructed"); 
}

}
