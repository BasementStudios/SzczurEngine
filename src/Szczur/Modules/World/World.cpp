#include "World.hpp"

/** @file World.cpp
 ** @description Implementation file with main class of the World module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Utility/SFML3D/Drawable.hpp"

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
	
}

/// update
void World::update(float deltaTime)
{
	
}

/// render
void World::render()
{
	this->objects.forEach<sf3d::Drawable>([](auto&& drawable) {
		drawable.draw();
	});
}

}
