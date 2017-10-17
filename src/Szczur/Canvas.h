#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
	class Canvas {
	// Members
	public:
		enum Layer {
			Ground, Flower, Tree, Roof, Effects, HUD, UI
		};
			
	private:
		sf::RenderTexture layers[7];
		sf::Vector2f windowSize;
		
	// Memeber functions	
	public:
		void resize(int _width, int _height);
		void render(sf::RenderTarget &target);
		void render(sf::Drawable& sprite, int layer);
		const sf::Vector2f& getWindowSize();
	};
}