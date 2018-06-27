#pragma once
#include <vector>
#include "Szczur/Modules/GUI/Base/BaseBar.hpp"
#include "SFML/Graphics.hpp"

namespace rat {
	class EquipmentObject; class ImageWidget; class Equipment;
	class AmuletSlot : public BaseBar
	{
	public:
		AmuletSlot(sf::Texture* slotText, sf::Texture* upText, sf::Texture* downText, sf::Vector2u slotSize, Equipment* equipment);
		void addAmulet(EquipmentObject* amulet);
		bool removeAmulet(std::string name);

		EquipmentObject* getChosenAmulet();
		std::vector<EquipmentObject*> getAmuletList();

		void leftArrowClicked();
		void rightArrowClicked();
	private:
		std::vector<EquipmentObject*> _amulets;

		EquipmentObject* _chosenAmulet;
		ImageWidget* _slot;
		ImageWidget* _amuletImage;
		ImageWidget* _leftArrow;
		ImageWidget* _rightArrow;

		sf::Vector2u _slotSize;
	};
}