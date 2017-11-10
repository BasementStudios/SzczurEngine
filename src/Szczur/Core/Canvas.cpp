#include "Canvas.hpp"

#include <tuple>
#include <vector>

namespace rat {
	void Canvas::init(sf::RenderWindow* windowPtr) {
		_windowPtr = windowPtr;

		for (auto& av : _layers)
			av.reset(new rat::RenderLayer(_windowPtr->getSize()));
	}

	void Canvas::recreateLayers() {
		for (auto& av : _layers)
			av->recreate(_windowPtr->getSize());
	}

	sf::RenderWindow& Canvas::getWindow() {
		return *_windowPtr;
	}

	const sf::RenderWindow& Canvas::getWindow() const {
		return *_windowPtr;
	}

	rat::RenderLayer& Canvas::getLayer(LayerId layerId) {
		return *_layers[(size_t)layerId];
	}

	const rat::RenderLayer& Canvas::getLayer(LayerId layerId) const {
		return *_layers[(size_t)layerId];
	}

	void Canvas::draw(LayerId layerId, const sf::Drawable& drawable, const sf::RenderStates& states) {
		_layers[(size_t)layerId]->draw(drawable, states);
	}

	void Canvas::display() {
		for (auto& av : _layers)
			av->display(*_windowPtr);
	}
}
