#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

namespace rat {
	class EquipmentObject; class Widget; class EquipmentSlot; class AmuletSlot; class Equipment; class WearableItem; class GUI;
	class ArmorSlots
	{
	public:
		ArmorSlots(sf::Vector2f frameSize, Equipment* equipment);

		void initAssetsViaGUI(GUI& gui);

		void setParent(Widget* newBase);
		void setPropPosition(sf::Vector2f position);

		void setArmor(EquipmentObject* armor);
		void setWeapon(EquipmentObject* weapon);

		void addAmulet(WearableItem* amulet);
		bool removeAmulet(std::string& name);
		//removes all items
		void reset();

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