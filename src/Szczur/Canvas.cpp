#include "Canvas.h"

namespace rat {
	void Canvas::resize(int _width, int _height) {
		windowSize =  sf::Vector2f(_width, _height);
		for(auto& layer : layers) {
			layer.create(_width, _height);
		}
	}
	void Canvas::render(sf::RenderTarget &target) {
		sf::Sprite spr;
		for(auto& layer : layers) {
			layer.display();
			spr.setTexture(layer.getTexture());
			target.draw(spr);
			layer.clear(sf::Color(0,0,0,0));
		}
	}
	void Canvas::render(sf::Drawable& sprite, int layer) {
		layers[layer].draw(sprite);
	}
	const sf::Vector2f& Canvas::getWindowSize() {
		return windowSize;
	}
}