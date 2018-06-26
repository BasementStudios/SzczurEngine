#pragma once
#include "EquipmentObject.hpp"
#include "Szczur/Modules/GUI/Base/BaseBar.hpp"
#include <SFML/Graphics.hpp>
#include "Szczur/Modules/GUI/ImageWidget.hpp"

namespace rat {
	class Widget;
	class EquipmentSlot //: public BaseBar
	{
	public:
		EquipmentSlot();

		void setSize(const sf::Vector2u& size);
		void setSize(unsigned int width, unsigned int height);
		void setTexture(sf::Texture*);
		void setParent(Widget* parent);
		void setPosition(sf::Vector2f pos);

		void setItemPosition(sf::Vector2f pos);
		void resetItemPosition();

		void setItem(EquipmentObject*);
		EquipmentObject* getItem();
		void removeItem();

		size_t index;

		Widget* getItemWidget();
		sf::Vector2f getPosition();

	private:
		ImageWidget* _slotImage;
		ImageWidget* _itemImage;

		EquipmentObject* _itemPlaced{ nullptr };

		Widget* _base;
	};
}