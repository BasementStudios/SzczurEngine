#pragma once

#include <SFML/Graphics.hpp>

#include "Drawable.hpp"

namespace rat {
	class Layer {
	private:

		sf::RenderTexture _renderTexture;

	public:

		Layer() = default;

		void create(const sf::Vector2u& size) {
			_renderTexture.create(size.x, size.y);
		}

		const sf::Texture& getTexture() const {
			return _renderTexture.getTexture();
		}

		void draw(const sf::Drawable& drawable) {
			_renderTexture.draw(drawable);
		}

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
