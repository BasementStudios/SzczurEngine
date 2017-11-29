#include "PreparationScreen.hpp"

#include <iostream>

#include "Utility.hpp"
#include "AssetManager.hpp"

namespace rat
{
	PreparationScreen::PreparationScreen(const Player& player, const int& battler, const int& settings)
		: _player(player), _battler(battler), _settings(settings) {
		rat::AssetManager::load_sprites();

		// Setup elements

		unsigned int _elements_count = elements_c(_player.Elements);
		_elements.reserve(_elements_count);

		for (unsigned int i = 0; i < _player.Elements.size(); ++i) {
			for (unsigned int j = 0; j < _player.Elements[i]; ++j) {
				_elements.emplace_back(i);
			}
		}

		std::cout << _elements_count << std::endl;

		// Positions

		int width = 41;

		_arrow = std::make_unique<sf::Sprite>(rat::AssetManager::Arrow);
		_arrow->setPosition(width * _elements_count + 100.0f, 100.0f);

		_elements_sprites.reserve(_elements_count);
		for (unsigned int i = 0; i < _elements_count; ++i) {
			_elements_sprites.emplace_back(rat::AssetManager::ElementsSprites[_elements[i]]);
			_elements_sprites[i].setPosition(width * (i + 1) + 100.0f, width + 100.0f);
		}
	}

	void PreparationScreen::run() {
		
	}

	void PreparationScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(*_arrow);
		for (auto& element_sprite : _elements_sprites) {
			target.draw(element_sprite);
		}
	}
}