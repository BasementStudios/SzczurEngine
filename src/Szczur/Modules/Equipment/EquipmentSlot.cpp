#include "EquipmentSlot.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
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
	void EquipmentSlot::setSize(const sf::Vector2u& size)
	{
		_slotImage->setSize(size);
		_itemImage->setSize(size);
	}
	void EquipmentSlot::setSize(unsigned int width, unsigned int height)
	{
		setSize({ width, height });
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
		_itemImage->setPosition(pos + static_cast<sf::Vector2f>(_itemImage->getSize() / 2u));
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
	sf::Vector2f EquipmentSlot::getPosition() {
		return _base->getPosition();
	}
}