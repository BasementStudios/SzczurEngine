#include "EquipmentSlot.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Equipment.hpp"

namespace rat {
	EquipmentSlot::EquipmentSlot() {
		_base = new Widget();
		_slotImage = new ImageWidget();
		_itemImage = new ImageWidget();
		_base->add(_slotImage);
		_base->add(_itemImage);
	}
	void EquipmentSlot::setTexture(sf::Texture* text) {
		_slotImage->setTexture(text);
	}
	void EquipmentSlot::setSize(const sf::Vector2f& size)
	{
		_slotImage->setSize(size);
		_itemImage->setSize(size);
	}
	void EquipmentSlot::setPropSize(const sf::Vector2f& size)
	{
		_slotImage->setPropSize(size);
		_itemImage->setPropSize(size);
	}

	void EquipmentSlot::setItem(EquipmentObject* item) {
		_itemPlaced = item;
		_itemImage->setTexture(item->getTexture());
		_itemImage->resetColor();
	}

	void EquipmentSlot::removeItem() {
		_itemPlaced = nullptr;
		_itemImage->setColor(sf::Color::Color(0, 0, 0, 0));
	}

	EquipmentObject* EquipmentSlot::getItem() {
		return _itemPlaced;
	}

	void EquipmentSlot::setItemPosition(sf::Vector2f pos) {
		_itemImage->setPosition(pos + _itemImage->getSize() / 2.f);
	}
	void EquipmentSlot::resetItemPosition() {
		_itemImage->setPosition(sf::Vector2f(0.f, 0.f));
	}

	Widget* EquipmentSlot::getItemWidget() {
		return _itemImage;
	}

	void EquipmentSlot::setParent(Widget* parent) {
		parent->add(_base);
	}

	void EquipmentSlot::setPosition(sf::Vector2f pos) {
		_base->setPosition(pos);
	}

	void EquipmentSlot::setPropPosition(sf::Vector2f pos) {
		_base->setPropPosition(pos);
	}

	sf::Vector2f EquipmentSlot::getPosition() {
		return _base->getPosition();
	}

	void EquipmentSlot::setStatus(bool newState) {
		if (newState) {
			_slotImage->resetColor();
			isUseble = true;
		}
		else {
			removeItem();
			_slotImage->setColor(sf::Color::Color(170, 170, 170));
			isUseble = false;
		}
	}

	bool EquipmentSlot::getStatus(){
		return isUseble;
	}
}