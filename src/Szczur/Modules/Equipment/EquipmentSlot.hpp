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

		void setTexture(sf::Texture*);

		void setHighlightTexture(sf::Texture*);
		void setHighlight(bool state);

		void setItemColor(sf::Color color);
		void resetItemColor();
		void setItemPosition(sf::Vector2f pos);
		void resetItemPosition();
		void setItem(EquipmentObject*);
		EquipmentObject* getItem();
		void removeItem();
		ImageWidget* getItemWidget();
		Widget* getWidget();

		void setParent(Widget* parent);

		void setPosition(sf::Vector2f pos);
		void setPropPosition(sf::Vector2f pos);
		sf::Vector2f getPosition();

		void setPropOrigin(sf::Vector2f pos);
		
		void setSize(const sf::Vector2f& size);
		void setPropSize(const sf::Vector2f& size);

		bool getStatus();
		void setStatus(bool newState);

		size_t index;	

	private:
		bool isUsable = true;	//is this slot accessible

		ImageWidget* _slotImage;
		ImageWidget* _itemImage;
		ImageWidget* _highlightFrameImage;
		Widget* _nullWidget;	//null widget only for callbacks

		EquipmentObject* _itemPlaced{ nullptr };

		Widget* _base;
	};
}