#include "Canvas.hpp"

namespace rat
{

void Canvas::update(float deltaTime)
{
	(void)deltaTime;
}

void Canvas::render()
{
	for (auto& av : _layers)
		av->display(_window);
}

void Canvas::recreateLayers()
{
	for (auto& av : _layers)
		av->recreate(_window.getSize());
}

sf::RenderWindow& Canvas::getWindow()
{
	return _window;
}

const sf::RenderWindow& Canvas::getWindow() const
{
	return _window;
}

rat::RenderLayer& Canvas::getLayer(LayerId id)
{
	return *_layers[(size_t)id];
}

const rat::RenderLayer& Canvas::getLayer(LayerId id) const
{
	return *_layers[(size_t)id];
}

void Canvas::draw(LayerId id, const sf::Drawable& drawable, const sf::RenderStates& states)
{
	_layers[(size_t)id]->draw(drawable, states);
}

void Canvas::draw(LayerId id, const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states)
{
	_layers[(size_t)id]->draw(vertices, vertexCount, type, states);
}

}
