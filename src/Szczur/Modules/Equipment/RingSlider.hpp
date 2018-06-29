#pragma once
#include <vector>
#include "SFML/Graphics.hpp"

namespace rat {
	class EquipmentSlot; class Equipment; class Widget; class ScrollAreaWidget; class WearableItem;
	class RingSlider
	{
	public:
		RingSlider(sf::Texture* frameText, sf::Vector2f frameSize, sf::Texture* pathText, sf::Texture* boundsText, sf::Texture* scrollText, Equipment* equipment, float length);

		void addItem(WearableItem* item);

		void setParent(Widget* newBase);

		void setPosition(sf::Vector2f);

		Equipment* _equipment;

		void selectRing(EquipmentSlot* slot);
	private:	
		sf::Vector2f _slotSize;
		sf::Texture* _slotTexture;

		std::vector<EquipmentSlot*> _ringSlots;
		std::vector<EquipmentSlot*> _selectedSlots;
		int _maxSelectedRings = 1;

		ScrollAreaWidget* _scroll;
		Widget* _base;

		float _length;
		
	};
}