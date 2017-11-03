#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
	class RenderLayer {
	private:

		sf::RenderTexture _renderTexture;
		sf::RenderStates _states;
		std::size_t _priority;

	public:

		RenderLayer(const sf::Vector2u& size, std::size_t priority) :
		 	_priority(priority) {
			_renderTexture.create(size.x, size.y);
		}

		void recreate(const sf::Vector2u& size) {
			_renderTexture.create(size.x, size.y);
		}

		sf::RenderTexture& base() {
			return _renderTexture;
		}

		const sf::RenderTexture& base() const {
			return _renderTexture;
		}

		std::size_t getPriority() const {
			return _priority;
		}

		void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default) {
			_renderTexture.draw(drawable, states);
		}

		void display(sf::RenderTarget& target) {
			_renderTexture.display();
			sf::Sprite spr(_renderTexture.getTexture());
			target.draw(spr, _states);
			_renderTexture.clear(sf::Color::Transparent);
		}
	};
}
