#pragma once

#include <SFML/Graphics.hpp>

namespace rat
{

class RenderLayer
{
public:

	RenderLayer(const sf::Vector2u& size);

	void recreate(const sf::Vector2u& size);

	void setRenderStates(const sf::RenderStates& states);

	sf::RenderStates getRenderStates() const;

	void clear(const sf::Color& color = sf::Color::Transparent);

	void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
	void draw(const sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);

	void display(sf::RenderTarget& target);

private:

	sf::RenderTexture _renderTexture;
	sf::RenderStates _states;

};

}
