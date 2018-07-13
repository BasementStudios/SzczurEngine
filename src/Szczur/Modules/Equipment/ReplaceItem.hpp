#pragma once
#include "SFML/Graphics.hpp"

namespace rat {
	class WindowWidget; class EquipmentObject; class Widget; class TextAreaWidget; class ImageWidget; class TextWidget; class Equipment;
	class ReplaceItem 
	{
	public:
		ReplaceItem(sf::Texture* backgroundText, sf::Texture* backgroundDescriptionText, sf::Texture* cancelText, sf::Font* font, Equipment* eq);

		void close();
		void setItem(EquipmentObject*);
		void setParent(Widget* base);

		void minimalize();
		void maximize();

		void lowerPosition();
		void higherPosition();
	private:
		WindowWidget * _descriptionWidget{ nullptr };
		ImageWidget* _cancelButton{ nullptr };
		ImageWidget* _itemImage{ nullptr };

		TextAreaWidget* _itemName{ nullptr };
		TextAreaWidget* _itemDescription{ nullptr };
		TextAreaWidget* _cancelText{ nullptr };

		Widget* _base{ nullptr };
		Equipment* _equipment{nullptr};
		bool _isClosed = true;
	};
}