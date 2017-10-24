#pragma once

#include <SFML/Graphics.hpp>

#include <Szczur/Core/ModuleBase.h>
#include <Szczur/Core/Layer.h>

namespace rat {
	class Canvas : public ModuleBase<> { using ModuleBase::ModuleBase;
	public:

		enum Layer {
			Ground, Flower, Tree, Roof, Effects, HUD, UI
		};

	private:

		// TODO Stritch : do przerobienia dynamiczne warstwy, to wyżej wypieprzyć :p
		rat::Layer _layers[7];
		// TODO Stritch : zamiast tego & na sf::RenderWindow?
		sf::Vector2f _windowSize;

	public:

		void init() {

		}

		void resize(unsigned _width, unsigned _height) {
			_windowSize = sf::Vector2f(_width, _height);
			for (auto& layer : _layers) {
				layer.create(_width, _height);
			}
		}

		void display(sf::RenderTarget& target) {
			sf::Sprite spr;
			for (auto& layer : _layers) {
				layer.display();
				spr.setTexture(layer.getTexture());
				target.draw(spr);
				layer.clear(sf::Color::Transparent);
			}
		}

		void render(sf::Drawable& sprite, int layer) {
			_layers[layer].draw(sprite);
		}

		void render(Drawable& sprite, int layer) {
			_layers[layer].draw(sprite);
		}

		sf::Vector2f getWindowSize() {
			return _windowSize;
		}
	};
}
