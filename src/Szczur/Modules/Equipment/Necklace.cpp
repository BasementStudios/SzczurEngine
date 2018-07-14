#include "Necklace.hpp"
#include "WearableItem.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Equipment.hpp"

namespace rat {
	Necklace::Necklace(sf::Texture* necklaceText, Equipment* eq) 
		:_equipment(eq)
	{
		_necklaceImage = new ImageWidget;
		_addWidget(_necklaceImage);
		_necklaceImage->setTexture(necklaceText);

		for (size_t i = 0; i < 3; i++)
		{
			_stonesImages.push_back({new ImageWidget, nullptr});
			_necklaceImage->add(_stonesImages[i].image);
			_stonesImages[i].image->setPropSize({.063f,0.083f});

			_stonesImages[i].image->setCallback(Widget::CallbackType::onPress, [this, i](Widget* owner) {
				if (_stonesImages[i].item)
					_equipment->_stoneStatusChanged(_stonesImages[i].item, false);
			});

			_stonesImages[i].image->setCallback(Widget::CallbackType::onHoverIn, [this, i](Widget* owner) {
				if(_stonesImages[i].item)
				_equipment->enableItemPreview(_stonesImages[i].item);
			});

			_stonesImages[i].image->setCallback(Widget::CallbackType::onHoverOut, [this](Widget* owner) {
				_equipment->disableItemPreview();
			});
		}

		_stonesImages[0].image->setPropPosition(0.2f, 1.3f);
		_stonesImages[1].image->setPropPosition(0.8f, .7f);
		_stonesImages[2].image->setPropPosition(0.5f, .2f);
	}

	void Necklace::setPropSize(float x, float y) {
		_necklaceImage->setPropSize(x,y);
	}
	
	bool Necklace::addStone(WearableItem* stone) {
		for (auto& i : _stonesImages) {
			if (!i.item) {
				i.item = stone;
				i.image->setTexture(stone->getTexture());
				return true;
			}
		}
		return false;
	}

	bool Necklace::removeStone(WearableItem* stone) {
		for (auto& i : _stonesImages) {
			if (i.item == stone) {
				i.item = nullptr;
				i.image->removeTexture();
				return true;
			}
		}
		return false;
	}

	void Necklace::reset() {

	}
}