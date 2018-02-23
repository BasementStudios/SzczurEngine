#include "World.hpp"

/** @file World.cpp
 ** @description Implementation file with main class of the World module. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "Szczur/Debug/Logger.hpp"

namespace rat
{

/* Methods*/
/// init
void World::init()
{
	// @info Testing propouses
	// Kostka ;f
	sf::Texture* kostkatex = &(getModule<Assets>.load<sf::Texture>("Assets/Objects/Exodus/default.png"));
	this->objectTypes.emplace_back("Kostka", DisplayData {
		kostkatex,
		sf::Vector2f(0.f, 0.f),
		sf::IntRect(0, 0, kostkatex->getSize().x, kostkatex->getSize().y),
	});
	auto kostka = this->objectTypes.back();
	this->sceneObjects.emplace_back(kostka, "Exodus", "default");
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
	auto& target = getModule<Window>();

	for (auto& object : this->sceneObjects) {
		target.draw(object, this->sceneRenderStates);
	}
}

}
