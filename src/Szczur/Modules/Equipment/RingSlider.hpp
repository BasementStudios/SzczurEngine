#pragma once
#include <vector>
#include "SFML/Graphics.hpp"

namespace rat {
	class EquipmentSlot; class Equipment; class Widget; class ScrollAreaWidget; class WearableItem; class ImageWidget;
	class RingSlider
	{
	public:
		RingSlider(
			sf::Texture* frameText,
			sf::Vector2f frameSize,
			sf::Texture* pathText,
			sf::Texture* boundsText,
			sf::Texture* scrollText,
			Equipment* equipment,
			sf::Texture* border,
			float length
		);

		void addItem(WearableItem* item);

		void setParent(Widget* newBase);

		void setPropPosition(sf::Vector2f);

		Equipment* _equipment;

		void selectRing(EquipmentSlot* slot);
	private:	
		sf::Vector2f _slotSize;
		sf::Texture* _slotTexture;

		std::vector<EquipmentSlot*> _ringSlots;
		std::vector<EquipmentSlot*> _selectedSlots;
		int _maxSelectedRings = 3;

		ScrollAreaWidget* _scroll;
		Widget* _base;
		ImageWidget* _border;

		float _length;
		
	};
}