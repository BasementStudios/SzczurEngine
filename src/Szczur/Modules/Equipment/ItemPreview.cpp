#include "ItemPreview.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextAreaWidget.hpp"
#include "EquipmentObject.hpp"

namespace rat {
	ItemPreview::ItemPreview(sf::Texture* backroundText, sf::Vector2f size, sf::Font* font) : size(size)
	{
		_background = new ImageWidget;
		_background->setTexture(backroundText);
		
		_itemImage = new ImageWidget;
		_background->add(_itemImage);
		
		_itemName = new TextWidget;
		_background->add(_itemName);
		_itemName->setFont(font);
		_itemName->setColor(sf::Color::White);
		_itemName->setCharacterSize(20);

		_itemDescription = new TextAreaWidget;
		_background->add(_itemDescription);
		_itemDescription->setFont(font);
		_itemDescription->setColor(sf::Color::White);
		_itemDescription->setCharacterSize(15);

		_background->setSize(size);
		_itemImage->setSize(sf::Vector2f(size.y, size.y));

		_itemName->setPosition(sf::Vector2f(size.y + 10.f, 10.f));
		_itemName->setSize(sf::Vector2f(2 * size.y - 10, 20.f));

		_itemDescription->setTextSize(sf::Vector2u(18u, 4u));
		_itemDescription->setPosition(sf::Vector2f(size.y + 10.f, 20.f + _itemName->getCharacterSize()));
	}

	void ItemPreview::setParent(Widget* base) {
		base->add(_background);
	}

	void ItemPreview::minimalize() {
		
		_background->fullyDeactivate();
	}

	void ItemPreview::setPosition(sf::Vector2f pos) {
		_background->setPosition(pos);			
		
	}

	void ItemPreview::setItem(EquipmentObject* item) {
		_background->fullyActivate();

		_itemDescription->setString(item->getDescription());
		_itemName->setString(item->getName());
		_itemImage->setTexture(item->getTexture());
	}

}