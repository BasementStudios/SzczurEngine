#pragma once

#include <SFML/Graphics.hpp>

#include "Layer.h"

namespace rat {
	class Canvas {
	public:

		enum Layer {
			Ground, Flower, Tree, Roof, Effects, HUD, UI
		};

	private:

		// TODO Stritch : do przerobienia dynamiczne warstwy, to wyżej wypieprzyć :p
		rat::Layer _layers[7];
		// TODO Stritch : zamiast tego & na sf::RenderWindow?
		sf::Vector2f _windowSize;

	public:

		sf::Vector2f getWindowSize();

		void resize(unsigned width, unsigned height);

		void render(sf::RenderTarget& target);

		void render(sf::Drawable& sprite, int layer);	
		
		void render(Drawable& sprite, int layer);	
	};
}
