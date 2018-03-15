#include "World.hpp"

/** @file World.cpp
 ** @description Implementation file with main class of the World module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "Szczur/Debug.hpp"

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
	// @info Testing propouses
	this->maps.emplace_back();

	this->currentMap = &(this->maps.back());
	// Kostka ;f
	// sf::Texture* kostkatex = &(getModule<Assets>.load<sf::Texture>("Assets/Objects/Exodus/default.png"));
	// this->objectTypes.emplace_back("Kostka", DisplayData {
	// 	kostkatex,
	// 	sf::Vector2f(0.f, 0.f),
	// 	sf::IntRect(0, 0, kostkatex->getSize().x, kostkatex->getSize().y),
	// });
	// auto kostka = this->objectTypes.back();
	// this->sceneObjects.emplace_back(kostka, "Exodus", "default");
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
