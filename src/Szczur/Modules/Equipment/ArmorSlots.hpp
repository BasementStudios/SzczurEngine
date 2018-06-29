#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

namespace rat {
	class EquipmentObject; class Widget; class EquipmentSlot; class AmuletSlot; class Equipment; class WearableItem;
	class ArmorSlots
	{
	public:
		ArmorSlots(sf::Texture* frameText, sf::Vector2f frameSize, sf::Texture* upText, sf::Texture* downText, Equipment* equipment);

		void setParent(Widget* newBase);
		void setPosition(sf::Vector2f position);

		void setArmor(EquipmentObject* armor);
		void setWeapon(EquipmentObject* weapon);

		void addAmulet(WearableItem* amulet);
		bool removeAmulet(sf::String name);

		WearableItem* getChosenAmulet();

		EquipmentSlot* getArmorSlot();
		EquipmentSlot* getWeaponSlot();

		std::vector<WearableItem*> getAmuletList();
	private:
		EquipmentSlot* _armorSlot;
		EquipmentSlot* _weaponSlot;
		AmuletSlot* _amuletSlot;

		Widget* _base;
	};
}