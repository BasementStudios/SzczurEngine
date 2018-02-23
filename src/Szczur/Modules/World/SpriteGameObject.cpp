#include "SpriteGameObject.hpp"

/** @file SpriteGameObject.hpp
 ** @description Implementaion file with sprite object class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace rat
{

/* Properties */
/// Texture
const sf::Texture& SpriteGameObject::getTexture() const
{
	return this->type.spriteData.texture;
}

/// Origin
const sf::Vector2f SpriteGameObject::getOrigin() const
{
	return this->type.spriteData.origin;
}

/// TextureRect
const sf::IntRect SpriteGameObject::getTextureRect() const
{
	return this->type.spriteData.rectMap[this->stateID];
}

/// hook setState
virtual void SpriteGameObject::setState(const StateID_t state)
{
	GameObject::setState(state);
	this->updateVertices();
}


/* Operators */
SpriteGameObject::SpriteGameObject(const GameObjectType* type, const std::string& name, const std::string& state = "default", const Vector_t& position = {0.f, 0.f}, const Vector_t& speed = {0.f, 0.f})
	: GameObject(type, name, state, position, speed)
{
	this->updateVertices();
}



/* Methods */
void SpriteGameObject::updateVertices()
{
	sf::IntRect rect = this->getTextureRect();

	float width  = static_cast<float>(rect.width);
    float height = static_cast<float>(rect.height);
	float left   = static_cast<float>(rect.left);
    float top    = static_cast<float>(rect.top);
    float right  = left + width;
    float bottom = top + height;

	this->vertices[0].position  = sf::Vector2f(0,     0);
    this->vertices[1].position  = sf::Vector2f(0,     height);
    this->vertices[2].position  = sf::Vector2f(width, 0);
    this->vertices[3].position  = sf::Vector2f(width, height);

    this->vertices[0].texCoords = sf::Vector2f(left,  top);
    this->vertices[1].texCoords = sf::Vector2f(left,  bottom);
    this->vertices[2].texCoords = sf::Vector2f(right, top);
    this->vertices[3].texCoords = sf::Vector2f(right, bottom);
}

void SpriteGameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform
		.translate(this->getPosition())
		.translate(-(this->getOrigin()))
	;
	
	states.texture = this->getTexture();;

	target.draw(this->vertices, 4, sf::PrimitiveType::TriangleStrip, states);
}

}
