#include "ArmorSlots.hpp"
#include "EquipmentSlot.hpp"
#include "EquipmentObject.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "AmuletSlot.hpp"
#include "Equipment.hpp"
#include "WearableItem.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat {
	ArmorSlots::ArmorSlots(sf::Vector2f frameSize, Equipment* equipment) {
		_base = new Widget;
		_base->setPropSize({0.36f, 0.165f});
		
		_amuletSlot = new AmuletSlot({0.11f, 0.11f}, equipment);
		_amuletSlot->setPropPosition({1.f, 0.f});//sf::Vector2f(2 * (frameSize.x + 10), 0.f));
		_amuletSlot->setParent(_base);

		_weaponSlot = new EquipmentSlot;
		_weaponSlot->setParent(_base);
		_weaponSlot->setPropSize(frameSize);
		_weaponSlot->getItemWidget()->setCallback(Widget::CallbackType::onHoverIn, [equipment, this](Widget* owner) {
			if (this->getWeaponSlot()->getItem())
				equipment->enableItemPreview(this->getWeaponSlot()->getItem());
			_weaponSlot->setHighlight(true);
		});
		_weaponSlot->getItemWidget()->setCallback(Widget::CallbackType::onHoverOut, [equipment, this](Widget* owner) {
			if (this->getWeaponSlot()->getItem())
				equipment->disableItemPreview();
			_weaponSlot->setHighlight(false);
		});

		_armorSlot = new EquipmentSlot;
		_armorSlot->setParent(_base);
		_armorSlot->setPropSize(frameSize);
		_armorSlot->setPropPosition({0.5f, 0.f});//sf::Vector2f(frameSize.x + 10, 0.f));
		_armorSlot->getItemWidget()->setCallback(Widget::CallbackType::onHoverIn, [equipment, this](Widget* owner) {
			if (this->getArmorSlot()->getItem())
				equipment->enableItemPreview(this->getArmorSlot()->getItem());
			_armorSlot->setHighlight(true);
		});
		_armorSlot->getItemWidget()->setCallback(Widget::CallbackType::onHoverOut, [equipment, this](Widget* owner) {
			if (this->getArmorSlot()->getItem())
				equipment->disableItemPreview();
			_armorSlot->setHighlight(false);
		});
	}

	void ArmorSlots::initAssetsViaGUI(GUI& gui) {
		_weaponSlot->setTexture(gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"));
		_weaponSlot->setHighlightTexture(gui.getAsset<sf::Texture>("Assets/Equipment/highlight.png"));

		_armorSlot->setTexture(gui.getAsset<sf::Texture>("Assets/Equipment/slot.png"));
		_armorSlot->setHighlightTexture(gui.getAsset<sf::Texture>("Assets/Equipment/highlight.png"));

		_amuletSlot->initAssetsViaGUI(gui);
	}

	void ArmorSlots::setParent(Widget* newBase) {
		newBase->add(_base);
	}

	void ArmorSlots::setArmor(EquipmentObject* armor) {
		if (_armorSlot->getItem()) dynamic_cast<WearableItem*>(_armorSlot->getItem())->deactivate();
		_armorSlot->setItem(armor);
		dynamic_cast<WearableItem*>(armor)->activate();
	}

	void ArmorSlots::setPropPosition(sf::Vector2f position) {
		_base->setPropPosition(position);
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
	void ArmorSlots::reset() {
		_amuletSlot->reset();

		if (_armorSlot->getItem()) {
			dynamic_cast<WearableItem*>(_armorSlot->getItem())->deactivate();
			_armorSlot->removeItem();
		}
		
		if (_weaponSlot->getItem()) {
			dynamic_cast<WearableItem*>(_weaponSlot->getItem())->deactivate();
			_weaponSlot->removeItem();
		}
	}
}