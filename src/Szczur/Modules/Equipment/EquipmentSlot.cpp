#include "EquipmentSlot.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Equipment.hpp"

namespace rat {
	EquipmentSlot::EquipmentSlot() {
		_base = new Widget();
		_slotImage = new ImageWidget();
		_itemImage = new ImageWidget();
		_lockImage = new ImageWidget();
		_lockImage->setPropPosition(0.5f, 0.5f);
		_lockImage->fullyDeactivate();
		_shadowImage = new ImageWidget();
		_shadowImage->fullyDeactivate();
		_nullWidget = new Widget;

		_base->add(_slotImage);
		_base->add(_shadowImage);
		_base->add(_itemImage);	
		_base->add(_lockImage);
		_base->add(_nullWidget);

		//_itemImage->setPropPosition(0.3f, 0.4f);

		setHighlight(false);
	}
	void EquipmentSlot::setTexture(sf::Texture* text, sf::Texture* shadowText, sf::Texture* lockText) {
		_slotImage->setTexture(text);
		_shadowImage->setTexture(shadowText);
		_lockImage->setTexture(lockText);
	}

	void EquipmentSlot::setHighlight(bool state) {
		if (state)
			_slotImage->setColor(sf::Color::Color(140, 140, 140));
		else
			_slotImage->setColor(sf::Color(255u, 255u, 255u, 150u));
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
		_shadowImage->setSize(size);
		_lockImage->setSize(size / 2.f);
		_nullWidget->setSize(size);
	}
	void EquipmentSlot::setPropSize(const sf::Vector2f& size)
	{
		_slotImage->setPropSize(size);
		_itemImage->setPropSize({size.x + .003f, size.y + .004f});
		//_itemImage->setPropSize(size);
		_shadowImage->setPropSize(size);
		_lockImage->setPropSize(size / 2.f);
		_nullWidget->setPropSize(size);
		_itemImage->setPropPosition(-0.3f, -0.4f);
	}

	void EquipmentSlot::setItem(EquipmentObject* item) {
		_itemPlaced = item;
		if (item) {
			_shadowImage->fullyActivate();
			_itemImage->setTexture(item->getTexture());
			_itemImage->resetColor();
		}
		else {
			_shadowImage->fullyDeactivate();
			_itemImage->removeTexture();
		}
	}

	void EquipmentSlot::removeItem() {
		if (_itemPlaced) {
			_shadowImage->fullyDeactivate();
			delete _itemPlaced;
			_itemPlaced = nullptr;
			_itemImage->removeTexture();
		}
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

	sf::Vector2f EquipmentSlot::getGlobalPosition() {
		return _base->getGlobalPosition();
	}

	void EquipmentSlot::setStatus(bool newState) {
		if (newState) {
			_lockImage->fullyDeactivate();
			isUsable = true;
		}
		else {
			removeItem();
			_lockImage->fullyActivate();
			isUsable = false;
		}
	}

	bool EquipmentSlot::getStatus(){
		return isUsable;
	}
}