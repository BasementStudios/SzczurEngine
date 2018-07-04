#pragma once
#include <SFML/Graphics.hpp>

namespace rat {
	class ImageWidget; class TextWidget; class Widget; class EquipmentObject; class TextAreaWidget; class WindowWidget;
	class ItemPreview 
	{
	public:
		ItemPreview(sf::Texture* backgroundText, sf::Texture* backgroundDescriptionText, sf::Vector2f size, sf::Font* font);

		void minimalize();
		void setItem(EquipmentObject*);
		void setParent(Widget* base);
		void setPosition(sf::Vector2f pos);
	private:
		bool isObjectApllied;

		ImageWidget* _iconBackground{ nullptr };
		WindowWidget* _descriptionWidget{ nullptr };
		ImageWidget* _itemImage{ nullptr };
		TextWidget* _itemName{ nullptr };

		Widget* _base{nullptr};

		TextAreaWidget* _itemDescription;
		sf::Vector2f size;
	};
}