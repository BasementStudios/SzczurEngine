#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
	class Canvas {
	public:

		enum Layer {
			Ground, Flower, Tree, Roof, Effects, HUD, UI
		};

	private:

		// TODO Stritch : do przerobienia dynamiczne warstwy, to wyżej wypieprzyć :p
		sf::RenderTexture _layers[7];
		// TODO Stritch : zamiast tego & na sf::RenderWindow?
		sf::Vector2f _windowSize;

	public:

		sf::Vector2f getWindowSize();

		void resize(unsigned width, unsigned height);

		void render(sf::RenderTarget& target);

		void render(sf::Drawable& sprite, Layer layer);		
	};
}
