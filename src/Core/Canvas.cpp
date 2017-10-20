#include "Canvas.h"

namespace rat {
	void Canvas::resize(unsigned _width, unsigned _height) {
		_windowSize = sf::Vector2f(_width, _height);
		for (auto& layer : _layers) {
			layer.create(_width, _height);
		}
	}

	void Canvas::render(sf::RenderTarget& target) {
		sf::Sprite spr;
		for (auto& layer : _layers) {
			layer.display();
			spr.setTexture(layer.getTexture());
			target.draw(spr);
			layer.clear(sf::Color::Transparent);
		}
	}

	void Canvas::render(sf::Drawable& sprite, int layer) {
		_layers[layer].draw(sprite);
	}
	
	void Canvas::render(Drawable& sprite, int layer) {
		_layers[layer].draw(sprite);
	}

	sf::Vector2f Canvas::getWindowSize() {
		return _windowSize;
	}
}
