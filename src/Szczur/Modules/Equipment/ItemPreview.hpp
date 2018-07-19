#pragma once
#include <SFML/Graphics.hpp>

namespace rat {
	class ImageWidget; class Widget; class EquipmentObject; class TextAreaWidget; class WindowWidget; class InterfaceWidget; class EquipmentSlot;
	class ItemPreview 
	{
	public:
		ItemPreview(sf::Texture* backgroundDescriptionText, sf::Font* font, sf::Font* italicFont);

		void minimalize();
		void maximize();
		void setItem(EquipmentObject*, sf::Vector2f pos);
		void setParent(Widget* base);
	private:
		ImageWidget* _descriptionWidget{ nullptr };
		TextAreaWidget* _itemName{ nullptr };
		TextAreaWidget* _itemDescription = nullptr;
		TextAreaWidget* _itemLoreDescription = nullptr;

		Widget* _base{nullptr};
	};
}