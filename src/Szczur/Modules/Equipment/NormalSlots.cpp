#include "NormalSlots.hpp"
#include "EquipmentSlot.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Equipment.hpp"

namespace rat
{
	bool SortByIndex::operator() (const std::shared_ptr<EquipmentSlot> lhs, const std::shared_ptr<EquipmentSlot> rhs) const {
		return lhs->index > rhs->index;
	}

	NormalSlots::NormalSlots(unsigned int slotNumber, sf::Texture* frameText, sf::Texture* highlightText, sf::Vector2i frameSize, Equipment* equipment)
		: _slotAmount(slotNumber), _frameText(frameText), _frameSize(frameSize), _equipment(equipment)
	{
		_base = new Widget;
		_base->setPropSize({4 * 0.086f, _slotAmount / 4 * 0.086f });
		_base->makeChildrenUnresizable();
		size_t x = 0;
		size_t y = 0;
		for (size_t i = 0; i < _slotAmount; i++)
		{
			if (i % 4 == 0 && i != 0)
			{
				y++;
				x = 0;
			}
			std::shared_ptr<EquipmentSlot> newSlot = std::make_shared<EquipmentSlot>();
			_allSlots.push_back(newSlot);
			newSlot->index = i;
			_freeSlots.push(newSlot);
			newSlot->setParent(_base);
			newSlot->setTexture(_frameText);
			newSlot->setPropPosition({x * .33f, y * .25f});
			newSlot->setPropSize({0.078f, 0.078f});
			newSlot->setHighlightTexture(highlightText);
			newSlot->getWidget()->setCallback(Widget::CallbackType::onPress, [this, newSlot](Widget* owner) {
				if (newSlot->getStatus())
					this->_onMouseButtonPressed(newSlot);
			});
			newSlot->getWidget()->setCallback(Widget::CallbackType::onRelease, [this, newSlot](Widget* owner) {
				if (newSlot->getStatus())
					this->_onMouseButtonReleased();
			});
			newSlot->getItemWidget()->setCallback(Widget::CallbackType::onHoverIn, [this, newSlot, equipment](Widget* owner) {
				if(newSlot->getStatus())
					_slotDropped = newSlot;
				if (newSlot->getItem())
					equipment->enableItemPreview(newSlot->getItem());
				newSlot->setHighlight(true);
			});
			newSlot->getItemWidget()->setCallback(Widget::CallbackType::onHoverOut, [this, newSlot, equipment](Widget* owner) {
				if (newSlot->getStatus())
					_removeSlotDropped(newSlot);
				if (newSlot->getItem())
					equipment->disableItemPreview();
				newSlot->setHighlight(false);
			});
			x++;
		}
		_itemHeldWidget = new ImageWidget;
		_base->add(_itemHeldWidget);
		_itemHeldWidget->setPropSize({ 0.078f, 0.078f });

	}
	void NormalSlots::setParent(Widget* newBase) {
		newBase->add(_base);
	}
	bool NormalSlots::addItem(EquipmentObject* item) {
		if (_freeSlots.size() > 0) {
			_freeSlots.top()->setItem(item);
			_occupiedSlots.insert(itemMap_t::value_type(item->getNameId(), _freeSlots.top()));
			_freeSlots.pop();
			return true;
		}
		else {
			_equipment->_replaceNewItem(item);
			_itemForReplacing = item;
			return false;
		}
	}
	bool NormalSlots::removeItem(const std::string& itemNameId) {
		auto it = _occupiedSlots.find(itemNameId);
		if (it != _occupiedSlots.end())
		{
			it->second->removeItem();
			_freeSlots.push(it->second);
			_occupiedSlots.erase(it);
			
			return true;
		}
		else
			return false;
	}
	bool NormalSlots::removeItem(size_t _index) {
		for (auto it = _occupiedSlots.begin(); it != _occupiedSlots.end(); ++it) {
			if (it->second->index == _index && it->second->getItem()) {
				it->second->removeItem();
				_freeSlots.push(it->second);
				_occupiedSlots.erase(it);
				
				return true;
			}
		}
		return false;
	}

	bool NormalSlots::removeItem(const std::string& itemNameId, int quantity) {
		if (quantity == 0 || hasItem(itemNameId, quantity)) {
			size_t index = 0;
			for (auto it = _occupiedSlots.begin(); it != _occupiedSlots.end();) {
				if (it->first == itemNameId) {
					++index;
					it->second->removeItem();
					_freeSlots.push(it->second);
					it = _occupiedSlots.erase(it);
					
					if (index == quantity) {
						return true;
					}
				}
				else {
					++it;
				}				
			}	
			return index > 0;
		}
		return false;
	}

	bool NormalSlots::hasItem(const std::string& nameId, int quantity) {
		int currentQuantity = 0;
		if (quantity <= 0)
			return false;
		for (auto& i : _occupiedSlots) {
			if (i.first == nameId)
				currentQuantity++;
			if (currentQuantity == quantity)
				return true;
		}
		return false;
	}

	bool NormalSlots::hasItem(const std::string& nameId) {
		return _occupiedSlots.find(nameId) != _occupiedSlots.end();
	}

	void NormalSlots::setPropPosition(sf::Vector2f pos) {
		_base->setPropPosition(pos);
	}

	bool NormalSlots::useItem(const std::string& nameId) {
		if (auto it = _occupiedSlots.find(nameId);it  != _occupiedSlots.end()) {
			if (static_cast<UsableItem*>(it->second->getItem())->useItem())
			{
				removeItem(it->second->index);
				return true;
			}
		}
		return false;
	}

	sf::Vector2f NormalSlots::getPosition() {
		return _base->getPosition();
	}

	void NormalSlots::_onMouseButtonPressed(std::shared_ptr<EquipmentSlot> clickedObj) {
		if (clickedObj->getItem()) {
			//if we are replacing item
			if (_itemForReplacing) {
				removeItem(clickedObj->index);
				clickedObj->setItem(_itemForReplacing);
				_occupiedSlots.insert(std::make_pair(_itemForReplacing->getNameId(), clickedObj));
				_equipment->_stopReplacingItem(true);
			}
			else {
				//setting clicked slot and item
				_slotHeld = clickedObj;
				_itemHeld = clickedObj->getItem();

				//setting current mouse position which will be used for positioning picked item widget
				_originalMousePosition = sf::Mouse::getPosition() - static_cast<sf::Vector2i>(_slotHeld->getPosition());

				//activating and setting picked item widget
				_itemHeldWidget->fullyActivate();
				_itemHeldWidget->setPosition(_slotHeld->getPosition());
				_itemHeldWidget->setTexture(_itemHeld->getTexture());

				clickedObj->setItemColor(sf::Color::Color(255, 255, 255, 100));

				//turning off item preview
				_equipment->disableItemPreview();
				_equipment->canPreviewBeInstantiated = false;
			}
		}
	}
	void NormalSlots::_onMouseButtonReleased() {
		if(_slotHeld){
			if (_slotDropped) {
				
				//dropping item into the same slot
				if (_slotDropped == _slotHeld) {
					//resetting slot colour
					_slotHeld->resetItemColor();

					_itemHeld = nullptr;
					_slotHeld = nullptr;

					//deactivating picked item widget
					_itemHeldWidget->fullyDeactivate();
				}

				//dropping item in free slot
				else if (!_slotDropped->getItem() && _slotDropped->getStatus()) {
					//removing item from first slot
					removeItem(_slotHeld->index);

					//putting item into new slot
					_slotDropped->setItem(_itemHeld);
					//_freeSlots.erase(_freeSlots.begin());
					_freeSlots.remove(_slotDropped);
					_occupiedSlots.insert(std::make_pair(_slotDropped->getItem()->getNameId(), _slotDropped));

					_slotHeld = nullptr;
					_itemHeld = nullptr;

					//deactivating picked item widget
					_itemHeldWidget->fullyDeactivate();
				}

				//swaping items between slots
				else if (_slotDropped->getItem()) {
					//removing item from first slot
					removeItem(_slotHeld->index);
					_freeSlots.remove(_slotHeld);

					//putting second item into first slot
					_slotHeld->setItem(_slotDropped->getItem());
					_occupiedSlots.insert(std::make_pair(_slotHeld->getItem()->getNameId(), _slotHeld));

					//removing item from second slot
					removeItem(_slotDropped->index);
					_freeSlots.remove(_slotDropped);

					//putting first item into second slot
					_slotDropped->setItem(_itemHeld);
					_occupiedSlots.insert(std::make_pair(_itemHeld->getNameId(), _slotDropped));

					_slotHeld = nullptr;
					_itemHeld = nullptr;

					//deactivating picked item widget
					_itemHeldWidget->fullyDeactivate();
				}
			}
			//mouse isn't over any slot
			else {
				_slotHeld->resetItemColor();

				_slotHeld = nullptr;
				_itemHeld = nullptr;

				_itemHeldWidget->fullyDeactivate();
			}

			_equipment->canPreviewBeInstantiated = true;
		}
	}

	void NormalSlots::update(float deltaTime) {
		if (_slotHeld) {
			_itemHeldWidget->setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition() - _originalMousePosition));
		}
		_checkForDoubleClick(deltaTime);
	}

	void NormalSlots::_checkForDoubleClick(float& deltaTime) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !_isCountingToDoubleClickEnabled && !_isLeftMouseButtonPressed) {
			_isLeftMouseButtonPressed = true;
		}
		if (_isLeftMouseButtonPressed && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			_isLeftMouseButtonPressed = false;
			_isCountingToDoubleClickEnabled = true;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && _isCountingToDoubleClickEnabled && _timeFromLastClick <= 0.2f) {
			_isCountingToDoubleClickEnabled = false;
			_timeFromLastClick = 0.f;
			if (_slotDropped) {
				if (_slotDropped->getItem()) {
					if (dynamic_cast<UsableItem*>(_slotDropped->getItem())->useItem()) {
						removeItem(_slotDropped->index);
						_equipment->disableItemPreview();
						if (_itemForReplacing) {
							addItem(_itemForReplacing);
							_equipment->_stopReplacingItem(true);
						}
					}
				}
			}
		}
		else if (_isCountingToDoubleClickEnabled && _timeFromLastClick <= 0.2f)
			_timeFromLastClick += deltaTime;
		else if (_isCountingToDoubleClickEnabled && _timeFromLastClick >= 0.2f) {
			_isCountingToDoubleClickEnabled = false;
			_timeFromLastClick = 0.f;
		}
	}

	void NormalSlots::_removeSlotDropped(std::shared_ptr<EquipmentSlot> slot) {
		if (_slotDropped == slot) {
			_slotDropped = nullptr;
		}
	}

	itemMap_t NormalSlots::getItemMap() {
		return _occupiedSlots;
	}

	int NormalSlots::getFreeSlotsAmount() {
		return _freeSlots.size();
	}

	void NormalSlots::resizeSlots(size_t newSize) {
		if (newSize != _slotAmount && newSize <= _allSlots.size()) {
			if (newSize > _slotAmount) {
				for (size_t i = _slotAmount; i < newSize; i++)
				{
					_allSlots[i]->setStatus(true);
					_freeSlots.push(_allSlots[i]);
				}
			}
			else {
				for (size_t i = _allSlots.size(); i > 0; i--)
				{
					if (i != newSize) {
						if (_allSlots[i - 1]->getItem()) {
							_occupiedSlots.erase(_allSlots[i - 1]->getItem()->getNameId());
						}
						else {
							_freeSlots.remove(_allSlots[i - 1]);
						}
						_allSlots[i - 1]->setStatus(false);
					}
					else {
						break;
					}
				}
			}
		}
		_slotAmount = newSize;
	}

	int NormalSlots::getSlotsAmount() {
		return _slotAmount;
	}

	void NormalSlots::_stopReplacing() {
		_itemForReplacing = nullptr;
	}
}