#pragma once
#include <SFML/Graphics.hpp>

namespace rat {
	class ImageWidget; class Widget; class EquipmentObject; class TextAreaWidget; class WindowWidget; class InterfaceWidget;
	class ItemPreview 
	{
	public:
		ItemPreview(sf::Texture* backgroundDescriptionText, sf::Font* font);

		void minimalize();
		void setItem(EquipmentObject*);
		void setParent(Widget* base);
	private:
		WindowWidget* _descriptionWidget{ nullptr };
		TextAreaWidget* _itemName{ nullptr };

		Widget* _base{nullptr};

		TextAreaWidget* _itemDescription;
	};
}