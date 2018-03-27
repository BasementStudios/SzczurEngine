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
	return this->getSpriteType()->getOrigin(this->getCurrentState());
}

/// Veritces
const std::array<sf::Vertex, 4> SpriteObject::getVertices() const
{
	return this->getSpriteType()->getVertices(this->getCurrentState());
}

/// State
const SpriteObjectType::StateID_t SpriteObject::getCurrentState() const
{
    return this->currentStateID;
}
void SpriteObject::setCurrentState(const SpriteObjectType::StateID_t& stateID)
{
    this->currentStateID = stateID;
}
void SpriteObject::setCurrentState(const std::string& stateString)
{
    this->currentStateID = this->getSpriteType()->getStateID(stateString);
}
const std::string& SpriteObject::getCurrentStateString() const
{
    return this->getSpriteType()->getStateString(this->currentStateID);
}



/* Operators */
/// Full constructor
SpriteObject::SpriteObject(
	const SpriteObjectType* 			type, 
	const std::string& 					name, 
	const Object::Vector_t& 			position, 
	const Object::Vector_t& 			speed, 
	const SpriteObjectType::StateID_t&	stateID
)
	: Object::Object((ObjectType*)type, name, position, speed)
{
	this->setCurrentState(stateID);
}
/// Standard constructor
SpriteObject::SpriteObject(
	const SpriteObjectType* 			type, 
	const std::string& 					name, 
	const Object::Vector_t& 			position, 
	const std::string&					stateString,
	const Object::Vector_t& 			speed
)
	: Object::Object((ObjectType*)type, name, position, speed)
{
	this->setCurrentState(stateString);
}



/* Methods */
/// draw
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
