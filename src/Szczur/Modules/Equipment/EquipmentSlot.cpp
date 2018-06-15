#include "EquipmentSlot.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
namespace rat {
	EquipmentSlot::EquipmentSlot() {
		_slotImage = new ImageWidget();
		_itemImage = new ImageWidget();
		_addWidget(_itemImage);
		_itemImage->add(_slotImage);
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
		_itemImage->setTexture(item->getIcon());
	}

	void EquipmentSlot::removeItem() {
		_itemPlaced = nullptr;
		_itemImage->setTexture(NULL);
	}
}