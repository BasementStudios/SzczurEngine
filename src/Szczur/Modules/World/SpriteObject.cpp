#include "SpriteObject.hpp"

/** @file SpriteObject.cpp
 ** @description Implementation file with sprite object class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <array>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

#include "Szczur/Modules/World/World.hpp"
#include "Szczur/Modules/World/Object.hpp"
#include "Szczur/Modules/World/ObjectType.hpp"
#include "Szczur/Modules/World/SpriteObjectType.hpp"

namespace rat
{

/* Properties */
/// Texture
const sf::Texture& SpriteObject::getTexture() const
{
	return reinterpret_cast<const SpriteObjectType*>(this->getType())->getTexture();//kurwa.
}

/// Origin
const World::Vector_t SpriteObject::getOrigin() const
{
	return reinterpret_cast<const SpriteObjectType*>(this->getType())->getOrigin(this->getState());
}

/// Veritces
const std::array<sf::Vertex, 4> SpriteObject::getVertices() const
{
	return reinterpret_cast<const SpriteObjectType*>(this->getType())->getVertices(this->getState());
}



/* Operators */
SpriteObject::SpriteObject(const SpriteObjectType* type, const std::string& name, ObjectType::StateID_t stateID, World::Vector_t position, World::Vector_t speed)
	: Object::Object((ObjectType*)type, name, stateID, position, speed)
{}



/* Methods */
void SpriteObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform
		.translate(this->getPosition())
		.translate(-(this->getOrigin()))
	;
	// @todo Camera perspective transformation ;f
	
	states.texture = &(this->getTexture());

	target.draw(this->getVertices().data(), 4, sf::PrimitiveType::TriangleStrip, states);
}

}
