#include "NormalSlots.hpp"
#include "EquipmentSlot.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"

namespace rat 
{
	bool sortByIndex(const EquipmentSlot* lhs, const EquipmentSlot* rhs) {
		return lhs->index < rhs->index;
	}

	NormalSlots::NormalSlots(unsigned int slotNumber, sf::Texture* frameText, sf::Vector2u frameSize)
		: _slotNumber(slotNumber), _frameText(frameText), _frameSize(frameSize)
	{
		_base = new Widget;
		size_t x = 0;
		size_t y = 0;
		for (size_t i = 0; i < _slotNumber; i++)
		{		
			if (i % 5 == 0 && i != 0)
			{
				y++;
				x = 0;
			}		
			_freeSlots.push_back(new EquipmentSlot);
			_freeSlots[i]->index = i;
			_freeSlots[i]->setParent(_base);
			_freeSlots[i]->setTexture(_frameText);
			_freeSlots[i]->setPosition(sf::Vector2f((frameSize.x + 5) * x, (frameSize.y + 5) * y));
			_freeSlots[i]->setSize(_frameSize);		
			x++;
		}
	}
	void NormalSlots::setParent(Widget* newBase) {
		newBase->add(_base);
	}
	void NormalSlots::addItem(EquipmentObject* item) {
		if (_freeSlots.size() > 0) {
			_freeSlots[0]->setItem(item);
			_itemSlots.insert(itemMap::value_type(item->getName() ,_freeSlots[0]));
			_freeSlots.erase(_freeSlots.begin());
			std::sort(_freeSlots.begin(), _freeSlots.end(), sortByIndex);
		}
	}
	void NormalSlots::removeItem(sf::String itemName) {
		auto it = _itemSlots.find(itemName);
		if (it != _itemSlots.end())
		{
			it->second->removeItem();
			_freeSlots.push_back(it->second);
			_itemSlots.erase(it->first);
			std::sort(_freeSlots.begin(), _freeSlots.end(), sortByIndex);
		}
		else
			LOG_INFO("item with given name doesn't exist in equipment");
	}

	void NormalSlots::setPosition(sf::Vector2f pos) {
		_base->setPosition(pos);
	}

	sf::Vector2f NormalSlots::getPosition() {
		return _base->getPosition();
	}
}