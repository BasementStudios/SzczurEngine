#include "RenderLayer.hpp"

/** @file RemderLayer.hpp
 ** @description Implementaion file for RenderLayer class.
 ** @author Patryk (Stritch)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Szczur/Debug/Logger.hpp"

namespace rat
{

/* Properties */
/// Size
sf::Vector2u RenderLayer::getSize() const
{
	return this->_texture.getSize();
}
void RenderLayer::setSize(const sf::Vector2u& size)
{
	this->_texture.create(size.x, size.y);
	this->_texture.clear(sf::Color::Black);
}

/// RenderStates
const sf::RenderStates RenderLayer::getRenderStates() const
{
	return this->_states;
}
void RenderLayer::setRenderStates(const sf::RenderStates& states)
{
	this->_states = states;
}



/* Operators */
/// Constructor 
RenderLayer::RenderLayer()
{
	;
}
RenderLayer::RenderLayer(const sf::Vector2u& size)
{
	this->setSize(size);
}



/* Methods */
/// clear
void RenderLayer::clear(const sf::Color& color)
{
	this->_texture.clear(color);
}

/// draw
void RenderLayer::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
	this->_texture.draw(drawable, states);
}

/// draw
void RenderLayer::draw(const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states)
{
	this->_texture.draw(vertices, vertexCount, type, states);
}

/// display
void RenderLayer::display(sf::RenderTarget& target)
{
	// @todo zrobić to normalnie -,- wgle po co renderstates...?
	this->_texture.display();
	sf::Sprite spr(this->_texture.getTexture());
	target.draw(spr, this->_states);
	this->_texture.clear(sf::Color::Transparent);
}

}
