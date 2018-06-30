#include "AmuletSlot.hpp"
#include "WearableItem.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Equipment.hpp"

namespace rat {
	AmuletSlot::AmuletSlot(sf::Texture* slotText, sf::Texture* upText, sf::Texture* downText, sf::Vector2f slotSize, Equipment* equipment) {
		
		_slotSize = slotSize;

		_leftArrow = new ImageWidget;
		_leftArrow->setTexture(upText);
		_addWidget(_leftArrow);
		_leftArrow->setSize(sf::Vector2f(slotSize.x / 2.5f, slotSize.y / 2.5f));
		_leftArrow->setPosition(sf::Vector2f(0.f, slotSize.y + 5));

		_rightArrow = new ImageWidget;
		_rightArrow->setTexture(downText);
		_addWidget(_rightArrow);
		_rightArrow->setSize(sf::Vector2f(slotSize.x / 2.5f, slotSize.y / 2.5f));
		_rightArrow->setPosition(sf::Vector2f(slotSize.x - slotSize.x / 2.5f, slotSize.y + 5));

		_slot = new ImageWidget;
		_slot->setTexture(slotText);
		_addWidget(_slot);
		_slot->setSize(slotSize);

		_amuletImage = new ImageWidget;
		_addWidget(_amuletImage);
		_amuletImage->setSize(slotSize);

		_leftArrow->setCallback(Widget::CallbackType::onPress, [this](Widget* owner) {
			leftArrowClicked();
		});
		_rightArrow->setCallback(Widget::CallbackType::onPress, [this](Widget* owner) {
			rightArrowClicked();
		});

		_amuletImage->setCallback(Widget::CallbackType::onHoverOut, [equipment, this](Widget* owner) {
			if (getChosenAmulet())
				equipment->disableItemPreview();
		});

		_amuletImage->setCallback(Widget::CallbackType::onHoverIn, [equipment, this](Widget* owner) {
			if (getChosenAmulet())
				equipment->enableItemPreview(getChosenAmulet());
		});

		_amulets.push_back(nullptr);
	}
	void AmuletSlot::addAmulet(WearableItem* amulet) {
		_amulets.push_back(amulet);
		if (_chosenAmulet == nullptr) {
			_amuletImage->resetColor();
			_chosenAmulet = amulet;
			_chosenAmulet->activate();
			_amuletImage->setTexture(amulet->getTexture());
		}
	}
	bool AmuletSlot::removeAmulet(std::string nameId) {
		for (size_t i = 0; i < _amulets.size(); i++)
		{
			if (_amulets[i]->getName() == nameId) {
				if (_chosenAmulet->getName() == nameId) {
					if (_amulets[i - 1])		//there is some other amulet to display
					{
						_chosenAmulet->deactivate();
						_amuletImage->setTexture(_amulets[i - 1]->getTexture());
						_chosenAmulet = _amulets[i - 1];
						_chosenAmulet->activate();
						_amulets.erase(_amulets.begin() + i);
						return true;
					}
					else {
						_chosenAmulet->deactivate();
						_amuletImage->setColor(sf::Color::Color(0, 0, 0, 0));
						_amulets.erase(_amulets.begin() + 1);
						_chosenAmulet = _amulets[0];
						return true;
					}
				}
				else {
					_amulets.erase(_amulets.begin() + i);
					return true;
				}

			}
		}
		return false;
	}
	WearableItem* AmuletSlot::getChosenAmulet() {
		return _chosenAmulet;
	}
	std::vector<WearableItem*> AmuletSlot::getAmuletList() {
		return _amulets;
	}
	void AmuletSlot::leftArrowClicked() {
		for (size_t i = 0; i < _amulets.size(); i++)
		{
			if (_amulets[i] == _chosenAmulet) {
				if (i + 1 < _amulets.size()) {
					if (_amulets[i + 1]) {
						_amuletImage->resetColor();
						if (_chosenAmulet)
							_chosenAmulet->deactivate();
						_chosenAmulet = _amulets[i + 1];
						_chosenAmulet->activate();
						_amuletImage->setTexture(_chosenAmulet->getTexture());
						break;
					}
				}
				else {
					//setting nullptr as chosen amulet
					if(_chosenAmulet)
					_chosenAmulet->deactivate();
					_chosenAmulet = _amulets[0];
					_amuletImage->setColor(sf::Color::Color(255, 255, 255, 0));
					break;

				}
			}
		}
	}
	void AmuletSlot::rightArrowClicked() {
		for (size_t i = 0; i < _amulets.size(); i++)
		{
			if (_amulets[i] == _chosenAmulet) {
				if (i != 0) {
					if (_amulets[i - 1]) {
						_amuletImage->resetColor();
						if (_chosenAmulet)
							_chosenAmulet->deactivate();
						_chosenAmulet = _amulets[i - 1];
						_chosenAmulet->activate();
						_amuletImage->setTexture(_chosenAmulet->getTexture());
					}
					else {
						if (_chosenAmulet)
						_chosenAmulet->deactivate();
						_chosenAmulet = _amulets[i - 1];
						_amuletImage->setColor(sf::Color::Color(255, 255, 255, 0));
						break;
					}
				}
				else {
					if (_amulets[_amulets.size() - 1] && _amulets.size() > 1) {
						_amuletImage->resetColor();
						_chosenAmulet = _amulets[_amulets.size() - 1];
						_chosenAmulet->activate();
						_amuletImage->setTexture(_chosenAmulet->getTexture());
						break;
					}
				}
			}
		}
	}
	
}