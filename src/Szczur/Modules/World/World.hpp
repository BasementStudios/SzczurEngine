#pragma once

/** @file World.hpp
 ** @description Header file with main class of the World module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>

#include "Szczur/Utility/Module.hpp"
#include "Szczur/Debug/Logger.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Assets/Assets.hpp"
#include "SceneObject.hpp"

namespace rat {

/** @class World
 ** @description Manages objects in the game world.
 **/
class World : public Module<Window, Assets>
{
    using Module::Module;
    


    /* Types */
    using SceneObject_t = rat::SceneObject;
	
	template <typename TObjectSection>
	using Container_t = std::vector<TObjectSection>;
	


    /* Variables */
private:
    Container_t<SceneObject_t> 	_sceneObjects; // @todo . trzeba zrobić własny container na to lepszy ;f
	sf::RenderStates			_sceneRenderStates	= sf::RenderStates::Default;
	//Container_t<BackgroundObject_t> _backObjects; // @todo .
	//sf::RenderStates			_backRenderStates	= sf::RenderStates::Default;
	//Container_t<ForegroundObject_t> _foreObjects; // @todo .
	//sf::RenderStates			_foreRenderStates	= sf::RenderStates::Default;



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

	// @todo wczytywanie/zapisywanie? 
	// @todo ...
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
