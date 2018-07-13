#include "AmuletSlot.hpp"
#include "WearableItem.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Equipment.hpp"

namespace rat {
	AmuletSlot::AmuletSlot(sf::Vector2f slotSize, Equipment* equipment) {
		
		_slotSize = slotSize;

		setPropSize({ slotSize.x, 0.165f});

		_slot = new ImageWidget;
		//_slot->setTexture(slotText);
		_addWidget(_slot);
		_slot->setPropSize(slotSize);

		_amuletImage = new ImageWidget;
		_addWidget(_amuletImage);
		_amuletImage->setPropSize(slotSize);

		_leftArrow = new ImageWidget;
		//_leftArrow->setTexture(upText);
		_addWidget(_leftArrow);
		_leftArrow->setPropSize(sf::Vector2f(slotSize.x / 2.5f, slotSize.y / 2.5f));
		_leftArrow->setPropPosition({0.f, 1.f});//sf::Vector2f(0.f, slotSize.y));	// + 5)

		_rightArrow = new ImageWidget;
		//_rightArrow->setTexture(downText);
		_addWidget(_rightArrow);
		_rightArrow->setPropSize(sf::Vector2f(slotSize.x / 2.5f, slotSize.y / 2.5f));
		_rightArrow->setPropPosition({ 1.f, 1.f }); //sf::Vector2f(slotSize.x - slotSize.x / 2.5f, slotSize.y));

		_leftArrow->setCallback(Widget::CallbackType::onPress, [this](Widget* owner) {
			leftArrowClicked();
		});
		_rightArrow->setCallback(Widget::CallbackType::onPress, [this](Widget* owner) {
			rightArrowClicked();
		});

		_amuletImage->setCallback(Widget::CallbackType::onHoverOut, [equipment, this](Widget* owner) {
			if (getChosenAmulet())
				equipment->disableItemPreview();
			_highlight->fullyDeactivate();
		});

		_amuletImage->setCallback(Widget::CallbackType::onHoverIn, [equipment, this](Widget* owner) {
			if (getChosenAmulet())
				equipment->enableItemPreview(getChosenAmulet());
			_highlight->fullyActivate();
		});

		_highlight = new ImageWidget;
		_highlight->setPropSize(slotSize);
		_addWidget(_highlight);
		_highlight->fullyDeactivate();

		_amulets.push_back(nullptr);
	}

	void AmuletSlot::initAssetsViaGUI(GUI& gui) {
		_slot->setTexture(gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"));
		_rightArrow->setTexture(gui.getAsset<sf::Texture>("Assets/Equipment/rightArrow.png"));
		_leftArrow->setTexture(gui.getAsset<sf::Texture>("Assets/Equipment/leftArrow.png"));
		_highlight->setTexture(gui.getAsset<sf::Texture>("Assets/Equipment/highlight.png"));
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
			if (_amulets[i] && _amulets[i]->getName() == nameId) {
				if (_chosenAmulet->getName() == nameId) {
					if (_amulets.size() > 2)		//there is some other amulet to display
					{
						_chosenAmulet->deactivate();
						if (_amulets[i - 1]) {
							_amuletImage->setTexture(_amulets[i - 1]->getTexture());
							_chosenAmulet = _amulets[i - 1];
						}
						else {
							_amuletImage->setTexture(_amulets[i + 1]->getTexture());
							_chosenAmulet = _amulets[i + 1];
						}
						_chosenAmulet->activate();
						_amulets.erase(_amulets.begin() + i);
						return true;
					}
					else {
						_chosenAmulet->deactivate();
						_amuletImage->removeTexture();
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
					_amuletImage->setColor(sf::Color(255u, 255u, 255u, 0u));
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
						_amuletImage->setColor(sf::Color(255u, 255u, 255u, 0u));
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

	void AmuletSlot::reset() {
		for (size_t i = _amulets.size() - 1; i > 0; i--) {
			removeAmulet(_amulets[i]->getNameId());
		}
	}
	
}