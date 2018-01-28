#include "Canvas.hpp"

namespace rat
{

Canvas::~Canvas()
{
	LOG_INFO(this, " : Module Canvas destructed");
}

void Canvas::clear()
{
	for (auto& av : _layers)
		av->clear();
}

void Canvas::render()
{
	for (auto& av : _layers)
		av->display(getModule<Window>().getWindow());
}

void Canvas::recreateLayers()
{
	for (auto& av : _layers)
		av->recreate(getModule<Window>().getWindow().getSize());
}

RenderLayer& Canvas::getLayer(LayerID::Code id)
{
	return *_layers[id];
}

const RenderLayer& Canvas::getLayer(LayerID::Code id) const
{
	return *_layers[id];
}

void Canvas::draw(LayerID::Code id, const sf::Drawable& drawable, const sf::RenderStates& states)
{
	_layers[id]->draw(drawable, states);
}

void Canvas::draw(LayerID::Code id, const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states)
{
	_layers[id]->draw(vertices, vertexCount, type, states);
}

}
