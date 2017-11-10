#include "RenderLayer.hpp"

namespace rat {
	RenderLayer::RenderLayer(const sf::Vector2u& size) {
		_renderTexture.create(size.x, size.y);
	}

	void RenderLayer::recreate(const sf::Vector2u& size) {
		_renderTexture.create(size.x, size.y);
	}

	void RenderLayer::setRenderStates(const sf::RenderStates& states) {
		_states = states;
	}

	sf::RenderStates RenderLayer::getRenderStates() const {
		return _states;
	}

	void RenderLayer::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
		_renderTexture.draw(drawable, states);
	}

	void RenderLayer::display(sf::RenderTarget& target) {
		_renderTexture.display();
		sf::Sprite spr(_renderTexture.getTexture());
		target.draw(spr, _states);
		_renderTexture.clear(sf::Color::Transparent);
	}
}
