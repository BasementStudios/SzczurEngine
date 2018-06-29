#pragma once
#include <SFML/Graphics.hpp>

namespace rat {
	class ImageWidget; class TextWidget; class Widget; class EquipmentObject; class TextAreaWidget;
	class ItemPreview 
	{
	public:
		ItemPreview(sf::Texture* backgroundText, sf::Vector2f size, sf::Font* font);

		void minimalize();
		void setItem(EquipmentObject*);
		void setParent(Widget* base);
		void setPosition(sf::Vector2f pos);
	private:
		EquipmentObject* _item;
		bool isObjectApllied;

		ImageWidget* _background;
		ImageWidget* _itemImage;
		TextWidget* _itemName;
		TextAreaWidget* _itemDescription;
		sf::Vector2f size;
	};
}