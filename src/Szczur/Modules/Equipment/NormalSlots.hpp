#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
//#include <multimap>


namespace rat {	
	class EquipmentSlot; class EquipmentObject; class Widget;
	typedef std::multimap<sf::String, EquipmentSlot*> itemMap;

	class NormalSlots			//part of equipment for normal items looking like a grid
	{
	public:
		NormalSlots(unsigned int slotNumber, sf::Texture* frameText, sf::Vector2u frameSize);

		void addItem(EquipmentObject* item);
		void removeItem(sf::String itemName);
		void resizeSlots(unsigned int newSize);
		void setParent(Widget* newBase);

		void setPosition(sf::Vector2f);
		sf::Vector2f getPosition();

	private:
		unsigned int _slotNumber;
		sf::Vector2u _frameSize;

		Widget* _base;

		sf::Texture* _frameText;

		itemMap _itemSlots;		//slots with items
		std::vector<EquipmentSlot*> _freeSlots;

	};
}
