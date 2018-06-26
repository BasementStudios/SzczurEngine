#include "ArmorSlots.hpp"
#include "EquipmentSlot.hpp"
#include "EquipmentObject.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "AmuletSlot.hpp"
#include "Equipment.hpp"

namespace rat {
	ArmorSlots::ArmorSlots(sf::Texture* frameText, sf::Vector2u frameSize, sf::Texture* upArrow, sf::Texture* downArrow, Equipment* equipment) {
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

	void ArmorSlots::setArmor(EquipmentObject* weapon) {
		_armorSlot->setItem(weapon);
	}

	void ArmorSlots::setPosition(sf::Vector2f position) {
		_base->setPosition(position);
	}

	void ArmorSlots::addAmulet(EquipmentObject* amulet) {
		_amuletSlot->addAmulet(amulet);
	}

	void ArmorSlots::removeAmulet(sf::String name) {
		_amuletSlot->removeAmulet(name);
	}

	EquipmentObject* ArmorSlots::getChosenAmulet() {
		return _amuletSlot->getChosenAmulet();
	}

	std::vector<EquipmentObject*> ArmorSlots::getAmuletList() {
		return _amuletSlot->getAmuletList();
	}

	void ArmorSlots::setWeapon(EquipmentObject* weapon) {
		_weaponSlot->setItem(weapon);
	}

	EquipmentSlot* ArmorSlots::getArmorSlot() {
		return _armorSlot;
	}
	EquipmentSlot* ArmorSlots::getWeaponSlot() {
		return _weaponSlot;
	}
}