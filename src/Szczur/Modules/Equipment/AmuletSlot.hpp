#pragma once
#include <vector>
#include "Szczur/Modules/GUI/Base/BaseBar.hpp"
#include "SFML/Graphics.hpp"

namespace rat {
	class EquipmentObject; class ImageWidget;
	class AmuletSlot : public BaseBar
	{
	public:
		AmuletSlot(sf::Texture* slotText, sf::Texture* upText, sf::Texture* downText, sf::Vector2u slotSize);
		void addAmulet(EquipmentObject* amulet);
		void removeAmulet(sf::String name);

		EquipmentObject* getChosenAmulet();
		std::vector<EquipmentObject*> getAmuletList();

		float yOffset;

		void upArrowClicked();
		void downArrowClicked();
	private:
		std::vector<EquipmentObject*> _amulets;

		EquipmentObject* _chosenAmulet;
		ImageWidget* _slot;
		ImageWidget* _amuletImage;
		ImageWidget* _upArrow;
		ImageWidget* _downArrow;

		sf::Vector2u _slotSize;
	};
}