#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

namespace rat {
	class Widget; class ListWidget; class Widget; class GUI; class ImageWidget;
	class PPCount 
	{
	public:
		PPCount();

		void setPropPosition(sf::Vector2f prop);
		void setPropSize(sf::Vector2f size);

		void setParent(Widget* base);

		void setPPCount(int newCount);
		void setGoodSlotAmount(int newAmount);
		void setBrokenSlotAmount(int newAmount);

		void initGUI(GUI& gui);
	private:
		int _PPCount;
		int _brokenCount;
		int _slotCount = 15;

		Widget* _base;

		sf::Texture* _PPTexture;
		sf::Texture* _slotTexture;
		sf::Texture* _brokenSlotTexture;

		ListWidget* _PPListWidget;
		ListWidget* _slotListWidget;

		std::vector<ImageWidget*> _PPList;
		std::vector<ImageWidget*> _slotList;
	};
}