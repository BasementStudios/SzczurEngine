#include "World.hpp"

/** @file World.cpp
 ** @description Implementation file with main class of the World module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "Szczur/Debug.hpp"
#include "Szczur/Modules/DragonBones/ArmatureObject.hpp"
#include "Szczur/Modules/DragonBones/ArmatureObjectType.hpp"
#include "Szczur/Modules/World/SpriteObject.hpp"
#include "Szczur/Modules/World/SpriteObjectType.hpp"

namespace rat
{

/* Operators */
/// Constructor
World::World()
{
	LOG_INFO(this, ": World module initializing"); 
	this->init();
	LOG_INFO(this, ": World module initialized!"); 
}
/// Destructor
World::~World()
{
    LOG_INFO(this, ": World module destructed"); 
}



/* Methods*/
/// init
void World::init()
{
	this->currentMap = &(this->maps.emplace_back());

	// @info Testing propouses
	//auto cedmin = new ArmatureObjectType("Cedmin", getModule<DragonBones>().getFactory());
	//this->types.push_back(cedmin);
	auto dragonType = new ArmatureObjectType("Dragon", getModule<DragonBones>().getFactory());
	this->types.push_back(dragonType);
	auto stoneType = new SpriteObjectType("Stone");
	this->types.push_back(stoneType);

	//this->currentMap->sceneObjects.push_back(
	//	new ArmatureObject(cedmin, "Player", {0.f, 0.f}, "Cedmin_Idle_008")
	//);
	
	this->currentMap->sceneObjects.push_back(
		this->playerObject = new ArmatureObject(dragonType, "Smok", {100.f, 400.f}, "Dragon")
	);
	this->currentMap->sceneObjects.push_back(
		new SpriteObject(stoneType, "MyStone", {400.f, 300.f})
	);
}

/// update
void World::update(float deltaTime)
{
	currentMap->update(deltaTime);
}

/// render
void World::render()
{
	getModule<Window>().draw(*currentMap);
}

}
