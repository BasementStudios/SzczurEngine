#include "ItemPreview.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextAreaWidget.hpp"
#include "EquipmentObject.hpp"

namespace rat {
	ItemPreview::ItemPreview(sf::Texture* backroundText, sf::Vector2u size, sf::Font* font) : size(size)
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

		_itemDescription = new TextWidget;
		_background->add(_itemDescription);
		_itemDescription->setFont(font);
		_itemDescription->setColor(sf::Color::White);
		_itemDescription->setCharacterSize(15);
	}

	void ItemPreview::setParent(Widget* base) {
		base->add(_background);
	}

	void ItemPreview::minimalize() {
		_background->setSize(sf::Vector2u(0u,0u));		//it's retarded i know
		_itemImage->setSize(sf::Vector2u(0u, 0u));		//need to rework this

		_itemName->setString("");
		_itemName->setSize(sf::Vector2u(0u, 0u));

		_itemDescription->setString("");
		_itemDescription->setSize(sf::Vector2u(0u, 0u));
	}

	void ItemPreview::setPosition(sf::Vector2f pos) {
		_background->setPosition(pos);
	}

	void ItemPreview::setItem(EquipmentObject* item) {
		_background->setSize(size);
		_itemImage->setSize(sf::Vector2u(size.y, size.y));
		_itemImage->setTexture(item->getTexture());
		_itemName->setString(item->getName());
		_itemName->setPosition(sf::Vector2f(size.y + 10.f, 10.f));
		_itemName->setSize(sf::Vector2u(2* size.y - 10, 20u));

		_itemDescription->setString(item->getDescription());
		_itemDescription->setPosition(sf::Vector2f(size.y + 10.f, 20.f + _itemName->getCharacterSize()));
		_itemDescription->setSize(sf::Vector2u(2 * size.y - 10,50u));
	}

}