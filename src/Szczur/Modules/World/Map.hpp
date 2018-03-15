#pragma once

/** @file Map.hpp
 ** @description Header file with Map class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <vector>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Object.hpp"

namespace rat {

/** @class Map
 ** @inherit sf::Drawable
 ** @description Manages objects in the single map.
 **/
class Map : public sf::Drawable
{
	/* Variables */
private:
	std::vector<Object*>	sceneObjects;
	sf::RenderStates		sceneRenderStates	{sf::RenderStates::Default};



	/* Methods */
public:
	/// Update
	void update(float deltaTime);

	/// Draw
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};

}
