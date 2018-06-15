#pragma once
#include "EquipmentObject.hpp"
#include "Szczur/Modules/GUI/Base/BaseBar.hpp"
#include <SFML/Graphics.hpp>

namespace rat {
	class ImageWidget;
	class EquipmentSlot : public BaseBar
	{
	public:
		EquipmentSlot();

		void setSize(const sf::Vector2u& size);
		void setSize(unsigned int width, unsigned int height);
		void setTexture(sf::Texture*);

		void setItem(EquipmentObject*);
		EquipmentObject* getItem();
		void removeItem();

		size_t index;

	private:
		ImageWidget* _slotImage;
		ImageWidget* _itemImage;

		EquipmentObject* _itemPlaced{ nullptr };
	};
}