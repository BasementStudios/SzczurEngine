#pragma once
#include "EquipmentObject.hpp"
#include "Szczur/Modules/GUI/Base/BaseBar.hpp"
#include <SFML/Graphics.hpp>
#include "Szczur/Modules/GUI/ImageWidget.hpp"

namespace rat {
	class Widget; class Equipment;
	class EquipmentSlot
	{
	public:
		EquipmentSlot();

		void setSize(const sf::Vector2f& size);
		void setPropSize(const sf::Vector2f& size);
		void setTexture(sf::Texture*);
		void setParent(Widget* parent);
		void setPosition(sf::Vector2f pos);
		void setPropPosition(sf::Vector2f pos);

		void setItemPosition(sf::Vector2f pos);
		void resetItemPosition();

		void setItem(EquipmentObject*);
		EquipmentObject* getItem();
		void removeItem();

		bool getStatus();
		void setStatus(bool newState);

		size_t index;

		Widget* getItemWidget();
		sf::Vector2f getPosition();

	private:
		bool isUseble = true;

		ImageWidget* _slotImage;
		ImageWidget* _itemImage;

		EquipmentObject* _itemPlaced{ nullptr };

		Widget* _base;
	};
}