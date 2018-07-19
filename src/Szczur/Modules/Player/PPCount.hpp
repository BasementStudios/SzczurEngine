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
		void setHighlightedPPAmount(int newAmount);
		void setSlotsAmount(int goodAmount, int badAmount);

		void initGUI(GUI& gui);
	private:
		int _PPCount;
		int _brokenCount;
		int _goodCount;
		int _slotCount = 15;
		int _highlightedPP;

		Widget* _base = nullptr;

		sf::Texture* _PPTexture = nullptr;
		sf::Texture* _highlightPPTexture = nullptr;
		sf::Texture* _slotTexture = nullptr;
		sf::Texture* _brokenSlotTexture = nullptr;

		ListWidget* _PPListWidget = nullptr;
		ListWidget* _slotListWidget = nullptr;

		std::vector<ImageWidget*> _PPList;
		std::vector<ImageWidget*> _slotList;
	};
}