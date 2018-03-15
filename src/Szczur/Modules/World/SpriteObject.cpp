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

#include "Szczur/Modules/World/Object.hpp"
#include "Szczur/Modules/World/ObjectType.hpp"
#include "SpriteObjectType.hpp"

namespace rat
{

/* Properties */
/// SpriteType
const SpriteObjectType* SpriteObject::getSpriteType() const
{
	return reinterpret_cast<const SpriteObjectType*>(this->getType());
}

/// Texture
const sf::Texture& SpriteObject::getTexture() const
{
	return this->getSpriteType()->getTexture();//kurwa.
}

/// Origin
const Object::Vector_t SpriteObject::getOrigin() const
{
	return this->getSpriteType()->getOrigin(this->getState());
}

/// Veritces
const std::array<sf::Vertex, 4> SpriteObject::getVertices() const
{
	return this->getSpriteType()->getVertices(this->getState());
}

/// State
const SpriteObjectType::StateID_t SpriteObject::getState() const
{
    return this->stateID;
}
void SpriteObject::setState(const SpriteObjectType::StateID_t& stateID)
{
    this->stateID = stateID;
}
const std::string& SpriteObject::getStateString() const
{
    return this->getSpriteType()->getStateString(this->getState());
}
void SpriteObject::setState(const std::string& stateString)
{
    this->setState(this->getSpriteType()->getStateID(stateString));
}



/* Operators */
SpriteObject::SpriteObject(
	const SpriteObjectType* 			type, 
	const std::string& 					name, 
	const Object::Vector_t& 			position, 
	const Object::Vector_t& 			speed, 
	const SpriteObjectType::StateID_t&	stateID
)
	: Object::Object((ObjectType*)type, name, position, speed)
{
	this->setState(stateID);
}



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
