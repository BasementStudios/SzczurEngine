#pragma once

#include <SFML/Graphics.hpp>

#include "DataTypes/Drawable.h"

namespace rat {
	class Layer {
	private:
		sf::RenderTexture _renderTexture;
		sf::Vector2i _size;
	
	public:
	// Init
		Layer() = default;
	
	// Setters
		void create(unsigned width, unsigned height) {
			_size.x = width;
			_size.y = height;
			_renderTexture.create(width, height);
		}
		void create(const sf::Vector2i& size) {
			_size = size;
			_renderTexture.create(_size.x, _size.y);
		}
		
	// Getters
		const sf::Texture& getTexture() {
			_renderTexture.getTexture();
		}
		
	// Utilities
		// Render sf::Drawable object on layer
		void draw(const sf::Drawable& drawable) {
			_renderTexture.draw(drawable);
		}
		// Render Drawable object on layer (in Drawable is using draw(const sf::Drawable&)
		void draw(const Drawable& drawable) {
			drawable.draw(*this);
		}
		void display() {
			_renderTexture.display();
		}
		void clear(const sf::Color& color) {
			_renderTexture.clear(color);
		}
	};
}