#pragma once
#include "SFML/Graphics.hpp"

namespace rat {
	class WindowWidget; class EquipmentObject; class Widget; class TextAreaWidget; class ImageWidget; class TextWidget; class NormalSlots;
	class ReplaceItem 
	{
	public:
		ReplaceItem(sf::Texture* backgroundText, sf::Texture* backgroundDescriptionText, sf::Texture* cancelText, sf::Font* font, NormalSlots* slots);

		void minimalize();
		void setItem(EquipmentObject*);
		void setParent(Widget* base);
	private:
		WindowWidget * _descriptionWidget{ nullptr };
		ImageWidget* _cancelButton{ nullptr };
		ImageWidget* _itemImage{ nullptr };

		TextAreaWidget* _itemName{ nullptr };
		TextAreaWidget* _itemDescription{ nullptr };
		TextWidget* _cancelText{ nullptr };

		Widget* _base{ nullptr };
		NormalSlots* _slots{nullptr};
	};
}