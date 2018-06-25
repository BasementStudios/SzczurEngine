#include "NormalSlots.hpp"
#include "EquipmentSlot.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

namespace rat 
{
	bool sortByIndex(const EquipmentSlot* lhs, const EquipmentSlot* rhs) {
		return lhs->index < rhs->index;
	}

	NormalSlots::NormalSlots(unsigned int slotNumber, sf::Texture* frameText, sf::Vector2i frameSize)
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
			std::shared_ptr<EquipmentSlot> newSlot(_freeSlots[i]);
			newSlot->index = i;
			newSlot->setParent(_base);
			newSlot->setTexture(_frameText);
			newSlot->setPosition(sf::Vector2f((frameSize.x + 5) * x, (frameSize.y + 5) * y));
			newSlot->setSize(static_cast<sf::Vector2u>(_frameSize));
			newSlot->getSlotWidget()->setCallback(Widget::CallbackType::onPress, [this, newSlot](Widget* owner) {
				this->onMouseButtonPressed(newSlot); LOG_INFO("aaaaaaaaaa");
			});
			newSlot->getSlotWidget()->setCallback(Widget::CallbackType::onRelease, [this](Widget* owner) {
				this->onMouseButtonReleased();
			});
			newSlot->getSlotWidget()->setCallback(Widget::CallbackType::onHoverIn, [this, newSlot, i](Widget* owner) {_slotDropped = newSlot; /*LOG_INFO("in ", i);*/ });
			newSlot->getSlotWidget()->setCallback(Widget::CallbackType::onHoverOut, [this, i](Widget* owner) {_slotDropped = nullptr; /*LOG_INFO("out ", i);*/ });
			x++;
		}
		_itemHeldWidget = new ImageWidget;
		_base->add(_itemHeldWidget);

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

	void NormalSlots::onMouseButtonPressed(std::shared_ptr<EquipmentSlot> clickedObj) {	
		if (!_isMouseButtonHeld && clickedObj->getItem()) {
			_isMouseButtonHeld = true;
			_slotHeld = clickedObj;
			_itemHeld = clickedObj->getItem();
			_originalMousePosition = sf::Mouse::getPosition() - static_cast<sf::Vector2i>(_slotHeld->getPosition());// -_frameSize / 2;
			_itemHeldWidget->setPosition(_slotHeld->getPosition());	
			_itemHeldWidget->setTexture(_slotHeld->getItem()->getTexture());
			_itemHeldWidget->setSize(static_cast<sf::Vector2u>(_frameSize));
			_itemHeldWidget->resetColor();
			clickedObj->removeItem();
		}
	}
	void NormalSlots::onMouseButtonReleased() {
		if (_isMouseButtonHeld && _slotDropped && _slotDropped != _slotHeld && !_slotDropped->getItem()) {
			_itemSlots.erase(_itemHeld->getName());
			_freeSlots.push_back(_slotHeld.get());
			_slotDropped->setItem(_itemHeld);
			for (auto it = _freeSlots.begin(); it < _freeSlots.end(); ++it)
			{
				if (*it == _slotDropped.get()) {
					_freeSlots.erase(it);
					break;
				}
			}
			_itemSlots.insert(std::make_pair(_slotDropped->getItem()->getName(), _slotDropped.get()));
			_isMouseButtonHeld = false;
			_slotHeld = nullptr;
			_itemHeldWidget->setSize(sf::Vector2u(0u, 0u));
			_itemHeldWidget->setPosition(sf::Vector2f(0.f, 0.f));
		}
		else if(_isMouseButtonHeld) {
			_slotHeld->setItem(_itemHeld);
			_isMouseButtonHeld = false;
			_slotHeld = nullptr;
			_itemHeldWidget->setSize(sf::Vector2u(0u,0u));
			_itemHeldWidget->setPosition(sf::Vector2f(0.f, 0.f));
		}		
	}

	void NormalSlots::update() {
		if (_isMouseButtonHeld) {
			_itemHeldWidget->setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition() - _originalMousePosition));
		}
		/*if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			LOG_INFO("left mouse button clicked");
			auto pair = isMouseOverSlot(mousePosition, false);
			if (!isMouseButtonHeld && pair.first) {
				LOG_INFO("left mouse button clicked over slot");
				isMouseButtonHeld = true;
				itemHeld = pair.second;
				itemHeld->setItemPosition(static_cast<sf::Vector2f>(mousePosition));
			}
			else if (isMouseButtonHeld) {
				itemHeld->setItemPosition(static_cast<sf::Vector2f>(mousePosition));
			}
		}
		else {
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && isMouseButtonHeld) {
				auto pair = isMouseOverSlot(mousePosition, true);
				if (pair.first) {
					itemHeld->resetItemPosition();
					pair.second->setItem(itemHeld->getItem());
					itemHeld->removeItem();
					itemHeld = nullptr;
					isMouseButtonHeld = false;
				}
				else {
					itemHeld->resetItemPosition();
					itemHeld = nullptr;
					isMouseButtonHeld = false;
				}
			}
		}*/
	}
}