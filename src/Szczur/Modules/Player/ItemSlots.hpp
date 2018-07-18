#pragma once
#include <vector>
#include "SFML/Graphics.hpp"

namespace rat {
	class ListWidget; class ImageWidget; class GUI; class Widget;
	class ItemSlots
	{
	public:
		ItemSlots(GUI& gui);

		void initGUI(GUI& gui);
		void setParent(Widget* base);

		void setPropPosition(sf::Vector2f pos);

		void setItem(sf::Texture* text, const std::string& nameID);
		void removeItem(sf::Texture* text, const std::string& nameID);
	private:
		struct Slot {
			ImageWidget* _slotImage = nullptr;
			ImageWidget* _itemImage = nullptr;
			std::string _itemNameId;
		};

		sf::Texture* _slotTexture;

		Widget* _base = nullptr;
		ListWidget* _slotsListWidget = nullptr;

		std::vector<Slot*> _slotsList;
	};
}