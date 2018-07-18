#include "ItemSlots.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat {
	ItemSlots::ItemSlots(GUI& gui) {
		_base = new Widget;

		_slotsListWidget = new ListWidget;
		_base->add(_slotsListWidget);
		_slotsListWidget->makeHorizontal();
		_slotsListWidget->setPropBetweenPad(0.003f);

		initGUI(gui);

		for (size_t i = 0; i < 4; i++)
		{
			_slotsList.push_back(new Slot);
			
			_slotsList[i]->_slotImage = new ImageWidget;
			_slotsList[i]->_slotImage->setPropSize({ .082f, 0.109f });
			_slotsList[i]->_slotImage->setTexture(_slotTexture);
			_slotsListWidget->add(_slotsList[i]->_slotImage);

			_slotsList[i]->_itemImage = new ImageWidget;
			_slotsList[i]->_itemImage->setPropSize({ .082f, 0.109f });
			_slotsList[i]->_slotImage->add(_slotsList[i]->_itemImage);
		}
	}

	void ItemSlots::initGUI(GUI& gui) {
		_slotTexture = gui.getTexture("Assets/Player/item_slot.png");
	}

	void ItemSlots::setParent(Widget* base) {
		base->add(_base);
	}

	void ItemSlots::setItem(sf::Texture* text, const std::string& nameID) {
		for (size_t i = 0; i < 4; i++)
		{
			if (_slotsList[i]->_itemNameId == "") {
				_slotsList[i]->_itemNameId = nameID;
				_slotsList[i]->_itemImage->setTexture(text);
				return;
			}
		}
	}

	void ItemSlots::removeItem(sf::Texture* text, const std::string& nameID) {
		for (size_t i = 0; i < 4; i++)
		{
			if (_slotsList[i]->_itemNameId == nameID) {
				_slotsList[i]->_itemNameId = "";
				_slotsList[i]->_itemImage->removeTexture();
				return;
			}
		}
	}

	void ItemSlots::setPropPosition(sf::Vector2f pos) {
		_base->setPropPosition(pos);
	}

}