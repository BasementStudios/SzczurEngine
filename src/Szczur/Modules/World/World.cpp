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
	// @info For testing purposes
	{
		using stoneObject_t = Object<BaseObject, SpriteComponent>; // @todo deduction WIP :f
		
		auto& stoneDisplayData = this->spriteDisplayData.emplace_back("Stone");

		auto& stoneObject = this->objects.emplace<stoneObject_t>(BaseObject{"kamyk"}, SpriteComponent{&stoneDisplayData});

		//stoneObject.setPosition(1.11f, 11.1f, 111.f);
	}
}

/// update
void World::update(float deltaTime)
{
	;
}

/// render
void World::render()
{
	auto& target = getModule<Window>().getWindow();
	
	this->objects.forEach<sf3d::Drawable>([&target](auto&& drawable) {
		target.draw(drawable);
	});
}

}
