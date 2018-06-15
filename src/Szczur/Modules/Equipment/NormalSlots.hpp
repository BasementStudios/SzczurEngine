#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

//possible bug/ to do removing items based on their names not pointers

namespace rat {
	class EquipmentSlot; class EquipmentObject; class Widget;
	class NormalSlots //part of equipment for normal items looking like a grid
	{
	public:
		NormalSlots(unsigned int slotNumber, sf::Texture* frameText, sf::Vector2u frameSize);

		void addItem(EquipmentObject* item);
		void removeItem(EquipmentObject* item);
		void resizeSlots(unsigned int newSize);
		void setParent(Widget* newBase);

		void setPosition(sf::Vector2f);
		sf::Vector2f getPosition();

	private:
		unsigned int _slotNumber;
		sf::Vector2u _frameSize;

		Widget* _base;

		sf::Texture* _frameText;

		std::vector<EquipmentSlot*> _freeSlots;
		std::vector<EquipmentSlot*> _occupiedSlots;

	};
}
