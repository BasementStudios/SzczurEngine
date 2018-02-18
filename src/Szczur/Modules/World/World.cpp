#include "World.hpp"

/** @file World.cpp
 ** @description Implementation file with main class of the World module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "Szczur/Debug/Logger.hpp"
#include "Szczur/Modules/Canvas/Canvas.hpp"

namespace rat
{

/* Methods*/
/// init
void World::init()
{
	this->_sceneObjects.emplace_back(&getModule<Assets>(), "Exodus", "default");
}

/// update
void World::update(float deltaTime)
{
	for (auto& object : this->_sceneObjects) {
		object.update(deltaTime);
	}
}

/// render
void World::render()
{
	auto& target = getModule<Window>().getWindow();

	for (auto& object : this->_sceneObjects) {
		object.draw(target, this->_sceneRenderStates);
	}
}

}
