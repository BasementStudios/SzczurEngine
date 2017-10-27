#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "Szczur/Utils/Hash.hpp"
#include "Szczur/Utils/HashVector.hpp"
#include "ModuleBase.hpp"
#include "Graphics/Layer.hpp"

namespace rat {
	class Canvas : public ModuleBase<> { using ModuleBase::ModuleBase;
	public:

		using Holder_t = rat::HashVector<rat::Hash32_t, std::unique_ptr<rat::Layer>>;

	private:

		Holder_t _layers;
		sf::Vector2u _size;

	public:

		void init() {

		}

		void addLayer(rat::Hash32_t layerId) {
			_layers.emplace(layerId, new Layer);
			_layers.sort();
		}

		void resize(const sf::Vector2u& size) {
			_size = size;
			for (auto& layer : _layers) {
				std::get<1>(layer)->create(size);
			}
		}

		void resize(const sf::RenderWindow& window) {
			resize(window.getSize());
		}

		void display(sf::RenderTarget& target, rat::Hash32_t layerId) {
			sf::Sprite spr;
			_layers[layerId]->display();
			spr.setTexture(_layers[layerId]->getTexture());
			target.draw(spr);
			_layers[layerId]->clear(sf::Color::Transparent);
		}

		void render(sf::Drawable& drawable, rat::Hash32_t layerId) {
			_layers[layerId]->draw(drawable);
		}

		void render(Drawable& drawable, rat::Hash32_t layerId) {
			_layers[layerId]->draw(drawable);
		}

		Layer& getLayer(rat::Hash32_t layerId) {
			return *_layers[layerId];
		}

		const Layer& getLayer(rat::Hash32_t layerId) const {
			return *_layers[layerId];
		}

		sf::Vector2u getLayersSize() const {
			return _size;
		}
	};
}
