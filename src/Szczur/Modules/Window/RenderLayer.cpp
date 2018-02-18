#include "RenderLayer.hpp"

namespace rat
{

RenderLayer::RenderLayer(const sf::Vector2u& size)
{
	recreate(size);
}

void RenderLayer::recreate(const sf::Vector2u& size)
{
	_renderTexture.create(size.x, size.y);
	_states = sf::RenderStates{};
	_sprite.setTexture(_renderTexture.getTexture());
}

void RenderLayer::setRenderStates(const sf::RenderStates& states)
{
	_states = states;
}

const sf::RenderStates& RenderLayer::getRenderStates() const
{
	return _states;
}

void RenderLayer::clear(const sf::Color& color)
{
	_renderTexture.clear(color);
}

void RenderLayer::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
	_renderTexture.draw(drawable, states);
}

void RenderLayer::draw(const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states)
{
	_renderTexture.draw(vertices, vertexCount, type, states);
}

void RenderLayer::display(sf::RenderTarget& target)
{
	_renderTexture.display();
	target.draw(_sprite, _states);
}

}
