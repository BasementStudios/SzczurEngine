#pragma once
#include <vector>
#include "Szczur/Modules/GUI/Base/BaseBar.hpp"
#include "SFML/Graphics.hpp"

namespace rat {
	class WearableItem; class ImageWidget; class Equipment; class GUI;
	class AmuletSlot : public BaseBar
	{
	public:
		AmuletSlot(sf::Vector2f slotSize, Equipment* equipment);
		void addAmulet(WearableItem* amulet);
		bool removeAmulet(std::string name);

		void initAssetsViaGUI(GUI& gui);

		WearableItem* getChosenAmulet();
		std::vector<WearableItem*> getAmuletList();

		void leftArrowClicked();
		void rightArrowClicked();
	private:
		std::vector<WearableItem*> _amulets;

		WearableItem* _chosenAmulet = nullptr;
		ImageWidget* _slot = nullptr;
		ImageWidget* _highlight = nullptr;
		ImageWidget* _amuletImage = nullptr;
		ImageWidget* _leftArrow = nullptr;
		ImageWidget* _rightArrow = nullptr;

		sf::Vector2f _slotSize;
	};
}