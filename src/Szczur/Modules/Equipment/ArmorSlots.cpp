#include "ArmorSlots.hpp"
#include "EquipmentSlot.hpp"
#include "EquipmentObject.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "AmuletSlot.hpp"
#include "Equipment.hpp"
#include "WearableItem.hpp"

namespace rat {
	ArmorSlots::ArmorSlots(sf::Texture* frameText, sf::Vector2f frameSize, sf::Texture* upArrow, sf::Texture* downArrow, Equipment* equipment) {
		_base = new Widget;
		
		_amuletSlot = new AmuletSlot(frameText, upArrow, downArrow, frameSize, equipment);
		_amuletSlot->setPosition(sf::Vector2f(2 * (frameSize.x + 10), 0.f));
		_amuletSlot->setParent(_base);

		_weaponSlot = new EquipmentSlot;
		_weaponSlot->setParent(_base);
		_weaponSlot->setSize(frameSize);
		_weaponSlot->setTexture(frameText);
		_weaponSlot->getItemWidget()->setCallback(Widget::CallbackType::onHoverIn, [equipment, this](Widget* owner) {
			if (this->getWeaponSlot()->getItem())
				equipment->enableItemPreview(this->getWeaponSlot()->getItem());
		});
		_weaponSlot->getItemWidget()->setCallback(Widget::CallbackType::onHoverOut, [equipment, this](Widget* owner) {
			if (this->getWeaponSlot()->getItem())
				equipment->disableItemPreview();
		});

		_armorSlot = new EquipmentSlot;
		_armorSlot->setParent(_base);
		_armorSlot->setSize(frameSize);
		_armorSlot->setTexture(frameText);
		_armorSlot->setPosition(sf::Vector2f(frameSize.x + 10, 0.f));
		_armorSlot->getItemWidget()->setCallback(Widget::CallbackType::onHoverIn, [equipment, this](Widget* owner) {
			if (this->getArmorSlot()->getItem())
				equipment->enableItemPreview(this->getArmorSlot()->getItem());
		});
		_armorSlot->getItemWidget()->setCallback(Widget::CallbackType::onHoverOut, [equipment, this](Widget* owner) {
			if (this->getArmorSlot()->getItem())
				equipment->disableItemPreview();
		});
	}

	void ArmorSlots::setParent(Widget* newBase) {
		newBase->add(_base);
	}

	void ArmorSlots::setArmor(EquipmentObject* armor) {
		if (_armorSlot->getItem()) dynamic_cast<WearableItem*>(_armorSlot->getItem())->deactivate();
		_armorSlot->setItem(armor);
		dynamic_cast<WearableItem*>(armor)->activate();
	}

	void ArmorSlots::setPosition(sf::Vector2f position) {
		_base->setPosition(position);
	}

	void ArmorSlots::addAmulet(WearableItem* amulet) {
		_amuletSlot->addAmulet(amulet);
	}

	bool ArmorSlots::removeAmulet(sf::String name) {
		return _amuletSlot->removeAmulet(name);
	}

	WearableItem* ArmorSlots::getChosenAmulet() {
		return _amuletSlot->getChosenAmulet();
	}

	std::vector<WearableItem*> ArmorSlots::getAmuletList() {
		return _amuletSlot->getAmuletList();
	}

	void ArmorSlots::setWeapon(EquipmentObject* weapon) {
		if (_weaponSlot->getItem()) dynamic_cast<WearableItem*>(_weaponSlot->getItem())->deactivate();
		_weaponSlot->setItem(weapon);
		dynamic_cast<WearableItem*>(weapon)->activate();
	}

	EquipmentSlot* ArmorSlots::getArmorSlot() {
		return _armorSlot;
	}
	EquipmentSlot* ArmorSlots::getWeaponSlot() {
		return _weaponSlot;
	}
}