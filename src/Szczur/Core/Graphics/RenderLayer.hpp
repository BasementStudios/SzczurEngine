#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
	class RenderLayer {
	private:

		sf::RenderTexture _renderTexture;
		sf::RenderStates _states;

	public:

		RenderLayer(const sf::Vector2u& size);

		void recreate(const sf::Vector2u& size);

		void setRenderStates(const sf::RenderStates& states);

		sf::RenderStates getRenderStates() const;

		void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);

		void display(sf::RenderTarget& target);
	};
}
