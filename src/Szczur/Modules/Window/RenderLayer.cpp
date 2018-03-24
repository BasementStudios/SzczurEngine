#include "RenderLayer.hpp"

/** @file RemderLayer.hpp
 ** @description Implementaion file for RenderLayer class.
 ** @author Patryk (Stritch)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace rat
{

/* Properties */
/// Size
sf::Vector2u RenderLayer::getSize() const
{
	return this->texture.getSize();
}
void RenderLayer::setSize(const sf::Vector2u& size)
{
	this->texture.create(size.x, size.y);
	this->texture.clear(sf::Color::Black);
}

/// RenderStates
const sf::RenderStates RenderLayer::getRenderStates() const
{
	return this->states;
}
void RenderLayer::setRenderStates(const sf::RenderStates& states)
{
	this->states = states;
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
	this->texture.clear(color);
}

/// draw
void RenderLayer::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
	this->texture.draw(drawable, states);
}

/// draw
void RenderLayer::draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states)
{
	this->texture.draw(vertices, vertexCount, type, states);
}

/// display
void RenderLayer::display(sf::RenderTarget& target)
{
	// @todo zrobić to normalnie -,- wgle po co renderstates...?
	this->texture.display();
	sf::Sprite spr(this->texture.getTexture());
	target.draw(spr, this->states);
	this->texture.clear(sf::Color::Transparent);
}

}
