#include "RenderLayer.hpp"

/** @file RemderLayer.hpp
 ** @description Implementaion file for RenderLayer class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace rat
{

// Size
sf::Vector2u RenderLayer::getSize() const
{
	return this->_texture.getSize();
}
void RenderLayer::setSize(const sf::Vector2u& size)
{
	this->_texture.create(size.x, size.y);
	this->_texture.clear(sf::Color::Black);
}

// RenderStates
const sf::RenderStates RenderLayer::getRenderStates() const
{
	return this->_states;
}
void RenderLayer::setRenderStates(const sf::RenderStates& states)
{
	this->_states = states;
}


// draw
void RenderLayer::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
	_texture.draw(drawable, states);
}

// draw
void RenderLayer::draw(const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states)
{
	_texture.draw(vertices, vertexCount, type, states);
}

// display
void RenderLayer::display(sf::RenderTarget& target)
{
	_texture.display();
	sf::Sprite spr(_texture.getTexture());
	target.draw(spr, _states);
	_texture.clear(sf::Color::Transparent);
}

}
