#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

namespace rat {
	class EquipmentObject; class Widget; class EquipmentSlot; class AmuletSlot;
	class ArmorSlots
	{
	public:
		ArmorSlots(sf::Texture* frameText, sf::Vector2u frameSize, sf::Texture* upText, sf::Texture* downText);

		void setParent(Widget* newBase);
		void setPosition(sf::Vector2f position);

		void setArmor(EquipmentObject* armor);
		void setWeapon(EquipmentObject* weapon);

		void addAmulet(EquipmentObject* amulet);
		void removeAmulet(sf::String name);

		EquipmentObject* getChosenAmulet();
		std::vector<EquipmentObject*> getAmuletList();
	private:
		EquipmentSlot* _armorSlot;
		EquipmentSlot* _weaponSlot;
		AmuletSlot* _amuletSlot;

		Widget* _base;
	};
}