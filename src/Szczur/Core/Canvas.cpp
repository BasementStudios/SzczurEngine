#include "Canvas.hpp"

#include <tuple>
#include <vector>

namespace rat {
	void Canvas::init(sf::RenderWindow* windowPtr) {
		_windowPtr = windowPtr;
	}

	void Canvas::recreateLayers() {
		for (auto& av : _layers)
			std::get<1>(av)->recreate(_windowPtr->getSize());
	}

	void Canvas::addLayer(rat::Hash32_t layerId, std::size_t priority) {
		_layers.emplace(layerId, new rat::RenderLayer(_windowPtr->getSize(), priority));
	}

	void Canvas::removeLayer(rat::Hash32_t layerId) {
		_layers.erase(layerId);
	}

	rat::RenderLayer& Canvas::getLayer(rat::Hash32_t layerId) {
		return *_layers.at(layerId);
	}

	const rat::RenderLayer& Canvas::getLayer(rat::Hash32_t layerId) const {
		return *_layers.at(layerId);
	}

	void Canvas::draw(rat::Hash32_t layerId, const sf::Drawable& drawable, const sf::RenderStates& states) {
		_layers.at(layerId)->draw(drawable, states);
	}

	void Canvas::display() {
		// TODO better
		std::vector<std::tuple<std::size_t, rat::RenderLayer*>> tmp;
		for (auto& av : _layers) {
			tmp.emplace_back(std::get<1>(av)->getPriority(), std::get<1>(av).get());
		}
		std::sort(tmp.begin(), tmp.end(), [](const auto& a, const auto& b) {
			return std::get<0>(a) > std::get<0>(b);
		});
		for (auto& av : tmp) {
			std::get<1>(av)->display(*_windowPtr);
		}
	}
}
