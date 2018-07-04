#include "EquipmentSlot.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Equipment.hpp"

namespace rat {
	EquipmentSlot::EquipmentSlot() {
		_base = new Widget();
		_slotImage = new ImageWidget();
		_itemImage = new ImageWidget();
		_highlightFrameImage = new ImageWidget();
		_nullWidget = new Widget;
		_base->add(_slotImage);
		_base->add(_itemImage);
		_base->add(_highlightFrameImage);
		_base->add(_nullWidget);

		setHighlight(false);
	}
	void EquipmentSlot::setTexture(sf::Texture* text) {
		_slotImage->setTexture(text);
	}

	void EquipmentSlot::setHighlightTexture(sf::Texture* text) {
		_highlightFrameImage->setTexture(text);
	}

	void EquipmentSlot::setHighlight(bool state) {
		if (state && isUsable)
			_highlightFrameImage->fullyActivate();
		else
			_highlightFrameImage->fullyDeactivate();
	}

	void EquipmentSlot::setItemColor(sf::Color color) {
		_itemImage->setColor(color);
	}

	void EquipmentSlot::resetItemColor() {
		_itemImage->resetColor();
	}

	void EquipmentSlot::setSize(const sf::Vector2f& size)
	{
		_slotImage->setSize(size);
		_itemImage->setSize(size);
		_highlightFrameImage->setSize(size);
		_nullWidget->setSize(size);
	}
	void EquipmentSlot::setPropSize(const sf::Vector2f& size)
	{
		_slotImage->setPropSize(size);
		_itemImage->setPropSize(size);
		_highlightFrameImage->setPropSize(size);
		_nullWidget->setPropSize(size);
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

	ImageWidget* EquipmentSlot::getItemWidget() {
		return _itemImage;
	}

	Widget* EquipmentSlot::getWidget() {
		return _nullWidget;
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

	void EquipmentSlot::setPropOrigin(sf::Vector2f newOrigin) {
		_base->setPropOrigin(newOrigin);
	}

	sf::Vector2f EquipmentSlot::getPosition() {
		return _base->getPosition();
	}

	void EquipmentSlot::setStatus(bool newState) {
		if (newState) {
			_slotImage->resetColor();
			isUsable = true;
		}
		else {
			removeItem();
			_slotImage->setColor(sf::Color::Color(170, 170, 170));
			isUsable = false;
		}
	}

	bool EquipmentSlot::getStatus(){
		return isUsable;
	}
}