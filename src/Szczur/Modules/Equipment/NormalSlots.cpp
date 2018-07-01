#include "NormalSlots.hpp"
#include "EquipmentSlot.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Equipment.hpp"

namespace rat
{
	bool sortByIndex(const EquipmentSlot* lhs, const EquipmentSlot* rhs) {
		return lhs->index < rhs->index;
	}

	NormalSlots::NormalSlots(unsigned int slotNumber, sf::Texture* frameText, sf::Vector2i frameSize, Equipment* equipment)
		: _slotAmount(slotNumber), _frameText(frameText), _frameSize(frameSize), _equipment(equipment)
	{
		_base = new Widget;
		size_t x = 0;
		size_t y = 0;
		for (size_t i = 0; i < _slotAmount; i++)
		{
			if (i % 4 == 0 && i != 0)
			{
				y++;
				x = 0;
			}
			_freeSlots.push_back(new EquipmentSlot());
			std::shared_ptr<EquipmentSlot> newSlot(_freeSlots[i]);
			_allSlots.push_back(newSlot.get());
			newSlot->index = i;
			newSlot->setParent(_base);
			newSlot->setTexture(_frameText);
			newSlot->setPosition(sf::Vector2f((frameSize.x + 5) * x, (frameSize.y + 5) * y)); //{ 0.045f, 0.08f});//
			newSlot->setSize(static_cast<sf::Vector2f>(_frameSize));
			newSlot->getItemWidget()->setCallback(Widget::CallbackType::onPress, [this, newSlot](Widget* owner) {
				if (newSlot->getStatus())
					this->onMouseButtonPressed(newSlot);
			});
			newSlot->getItemWidget()->setCallback(Widget::CallbackType::onRelease, [this, newSlot](Widget* owner) {
				if (newSlot->getStatus())
					this->onMouseButtonReleased();
			});
			newSlot->getItemWidget()->setCallback(Widget::CallbackType::onHoverIn, [this, newSlot, equipment](Widget* owner) {
				_slotDropped = newSlot;
				if (newSlot->getItem())
					equipment->enableItemPreview(newSlot->getItem());
			});
			newSlot->getItemWidget()->setCallback(Widget::CallbackType::onHoverOut, [this, newSlot, equipment](Widget* owner) {
				_removeSlotDropped(newSlot);
				if (newSlot->getItem())
					equipment->disableItemPreview();
			});
			x++;
		}
		_itemHeldWidget = new ImageWidget;
		_base->add(_itemHeldWidget);
		_itemHeldWidget->setSize(static_cast<sf::Vector2f>(_frameSize));

	}
	void NormalSlots::setParent(Widget* newBase) {
		newBase->add(_base);
	}
	void NormalSlots::addItem(EquipmentObject* item) {
		if (_freeSlots.size() > 0) {
			_freeSlots[0]->setItem(item);
			_itemSlots.insert(itemMap_t::value_type(item->getNameId(), _freeSlots[0]));
			_freeSlots.erase(_freeSlots.begin());
			std::sort(_freeSlots.begin(), _freeSlots.end(), sortByIndex);
		}
	}
	bool NormalSlots::removeItem(sf::String itemNameId) {
		auto it = _itemSlots.find(itemNameId);
		if (it != _itemSlots.end())
		{
			it->second->removeItem();
			_freeSlots.push_back(it->second);
			_itemSlots.erase(it);
			std::sort(_freeSlots.begin(), _freeSlots.end(), sortByIndex);
			return true;
		}
		else
			return false;
	}
	bool NormalSlots::removeItem(int _index) {
		for (auto it = _itemSlots.begin(); it != _itemSlots.end(); ++it) {
			if (it->second->index == _index && it->second->getItem()) {
				it->second->removeItem();
				_freeSlots.push_back(it->second);
				_itemSlots.erase(it);
				std::sort(_freeSlots.begin(), _freeSlots.end(), sortByIndex);
				return true;
			}
		}
		return false;
	}

	void NormalSlots::setPropPosition(sf::Vector2f pos) {
		_base->setPropPosition(pos);
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
			_itemHeldWidget->fullyActivate();
			_itemHeldWidget->setPosition(_slotHeld->getPosition());
			_itemHeldWidget->setTexture(_slotHeld->getItem()->getTexture());
			_itemHeldWidget->resetColor();
			clickedObj->setItemColor(sf::Color::Color(255, 255, 255, 100));
			_equipment->disableItemPreview();
			_equipment->canPreviewBeInstantiated = false;
		}
	}
	void NormalSlots::onMouseButtonReleased() {
		if (_slotDropped == _slotHeld && _isMouseButtonHeld && _slotDropped->getItem()) {
			_isMouseButtonHeld = false;
			_slotHeld->resetItemColor();
			_slotHeld = nullptr;
			_itemHeldWidget->fullyDeactivate();
		}
		else if (_isMouseButtonHeld && _slotDropped && _slotDropped != _slotHeld && !_slotDropped->getItem()&& _slotDropped->getStatus()) {	//placing item in free slot
			_slotHeld->removeItem();
			_itemSlots.erase(_itemHeld->getNameId());
			_freeSlots.push_back(_slotHeld.get());
			_slotDropped->setItem(_itemHeld);
			for (auto it = _freeSlots.begin(); it < _freeSlots.end(); ++it)
			{
				if (*it == _slotDropped.get()) {
					_freeSlots.erase(it);
					break;
				}
			}
			_itemSlots.insert(std::make_pair(_slotDropped->getItem()->getNameId(), _slotDropped.get()));
			_isMouseButtonHeld = false;
			_slotHeld = nullptr;
			_itemHeldWidget->fullyDeactivate();
		}
		else if (_isMouseButtonHeld && _slotDropped && _slotDropped != _slotHeld && _slotDropped->getItem()) {	//swaping items
			_slotHeld->removeItem();
			_slotHeld->setItem(_slotDropped->getItem());
			_slotDropped->setItem(_itemHeld);
			_isMouseButtonHeld = false;
			_slotHeld = nullptr;
			_itemHeldWidget->fullyDeactivate();
		}
		else if (_isMouseButtonHeld) {
			_isMouseButtonHeld = false;
			if(_slotHeld->getItem())
			_slotHeld->resetItemColor();
			_slotHeld = nullptr;
			_itemHeldWidget->fullyDeactivate();
		}
		_equipment->canPreviewBeInstantiated = true;
	}

	void NormalSlots::update(float deltaTime) {
		if (_isMouseButtonHeld) {
			_itemHeldWidget->setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition() - _originalMousePosition));
		}
		checkForDoubleClick(deltaTime);
	}

	void NormalSlots::checkForDoubleClick(float deltaTime) {
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
					LOG_INFO(_slotDropped->getItem()->getNameId(), _slotDropped->index);
					if (dynamic_cast<UsableItem*>(_slotDropped->getItem())->useItem()) {
						removeItem(_slotDropped->index);
						_equipment->disableItemPreview();
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
		return _itemSlots;
	}

	int NormalSlots::getFreeSlotsAmount() {
		return _freeSlots.size();
	}

	void NormalSlots::resizeSlots(size_t newSize) {
		if (newSize != _slotAmount) {
			if (newSize > _slotAmount) {
				for (size_t i = _slotAmount; i < newSize; i++)
				{
					_allSlots[i - 1]->setStatus(true);
					_freeSlots.push_back(_allSlots[i - 1]);
				}
			}
			else {
				for (size_t i = _allSlots.size(); i > 0; i--)
				{
					if (i != newSize) {
						if (_allSlots[i - 1]->getItem()) {
							_itemSlots.erase(_allSlots[i - 1]->getItem()->getNameId());
						}
						else {
							_freeSlots.erase(_freeSlots.begin() + i - 1);
						}
						_allSlots[i - 1]->setStatus(false);
					}
					else {
						break;
					}
				}
			}
		}
	}

	int NormalSlots::getSlotsAmount() {
		return _slotAmount;
	}
}