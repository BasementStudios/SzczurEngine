#pragma once
#include <vector>
#include "SFML/Graphics.hpp"

namespace rat {
	class EquipmentSlot; class Equipment; class Widget; class ScrollAreaWidget; class WearableItem; class WindowWidget; class GUI; class EquipmentObject; class ListWidget;
	class RingSlider
	{
		friend class Equipment;
	public:

		RingSlider(
			Equipment* equipment,
			GUI& gui,
			sf::Vector2f frameSize
		);

		void initAssetsViaGUI(GUI& gui);

		void addItem(WearableItem* item);

		void setParent(Widget* newBase);

		void setPropPosition(sf::Vector2f);
		void setPosition(sf::Vector2f);

		Equipment* _equipment;

		std::vector<EquipmentObject*> getSelectedRings();
		std::vector<EquipmentObject*> getStonesList();

		void selectRing(EquipmentSlot* slot);
		void reset();
	private:
		sf::Texture* _slotTexture;
		sf::Texture* _slotChosenTexture;
		sf::Texture* _shadowTexture;
		sf::Texture* _lockTexture;

		std::vector<EquipmentSlot*> _stoneSlots;
		std::vector<EquipmentSlot*> _selectedSlots;

		ScrollAreaWidget* _scroll = nullptr;
		Widget* _base = nullptr;
		ListWidget* _baseForItems = nullptr;
		std::vector<ListWidget*> _itemsLists;
		WindowWidget* _border = nullptr;

		void _takeStoneOf(WearableItem* stone);
		
	};
}