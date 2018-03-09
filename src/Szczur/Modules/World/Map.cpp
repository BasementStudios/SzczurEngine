#include "Map.hpp"

/** @file Map.cpp
 ** @description Implementation file with Map class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Object.hpp"

namespace rat
{

/* Methods */
/// update
void Map::update(float deltaTime)
{
	// for (auto& object : this->sceneObjects) {
	// 	object.update(deltaTime);
	// }
}

/// render
void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	for (auto& object : this->sceneObjects) {
		target.draw(*object, this->sceneRenderStates);
	}
}

}
