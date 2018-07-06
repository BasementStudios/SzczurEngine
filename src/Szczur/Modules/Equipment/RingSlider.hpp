#pragma once
#include <vector>
#include "SFML/Graphics.hpp"

namespace rat {
	class EquipmentSlot; class Equipment; class Widget; class ScrollAreaWidget; class WearableItem; class WindowWidget; class GUI; class EquipmentObject; class ListWidget;
	class RingSlider
	{
	public:

		RingSlider(
			sf::Vector2f frameSize,
			Equipment* equipment
		);

		void initAssetsViaGUI(GUI& gui);

		void addItem(WearableItem* item);

		void setParent(Widget* newBase);

		void setPropPosition(sf::Vector2f);
		void setPosition(sf::Vector2f);

		Equipment* _equipment;

		void setSelectedRingsLimit(int newSize);
		std::vector<EquipmentObject*> getSelectedRings();
		std::vector<EquipmentObject*> getRingsList();

		void selectRing(EquipmentSlot* slot);
	private:	
		sf::Vector2f _slotSize;
		sf::Texture* _slotTexture;
		sf::Texture* _slotChosenTexture;
		sf::Texture* _highlightTexture;

		std::vector<EquipmentSlot*> _ringSlots;
		std::vector<EquipmentSlot*> _selectedSlots;
		int _maxSelectedRings = 3;

		ScrollAreaWidget* _scroll;
		Widget* _base;
		ListWidget* _baseForItems;
		WindowWidget* _border;

		float _length;
		
	};
}