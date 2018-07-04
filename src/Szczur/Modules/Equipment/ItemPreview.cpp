#include "ItemPreview.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextAreaWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "EquipmentObject.hpp"

namespace rat {
	ItemPreview::ItemPreview(sf::Texture* backgroundIconText, sf::Texture* backgroundDescriptionText, sf::Vector2f size, sf::Font* font) : size(size)
	{
		_base = new Widget;

		_iconBackground = new ImageWidget;
		_iconBackground->setTexture(backgroundIconText);
		_iconBackground->setSize({size.y, size.y});
		_base->add(_iconBackground);
		
		_itemImage = new ImageWidget;
		_base->add(_itemImage);
		
		_descriptionWidget = new WindowWidget;
		_descriptionWidget->setSize({size.x - size.y, size.y});
		_descriptionWidget->setPosition({ size.y, 0.f });
		_base->add(_descriptionWidget);
		_descriptionWidget->setTexture(backgroundDescriptionText, 10);

		_itemName = new TextWidget;
		_descriptionWidget->add(_itemName);
		_itemName->setFont(font);
		_itemName->setColor(sf::Color::White);
		_itemName->setCharacterSize(20);

		_itemDescription = new TextAreaWidget;
		_descriptionWidget->add(_itemDescription);
		_itemDescription->setFont(font);
		_itemDescription->setColor(sf::Color::White);
		_itemDescription->setCharacterSize(15);

		_base->setSize(size);
		_itemImage->setSize(sf::Vector2f(size.y, size.y));

		_itemName->setPosition(sf::Vector2f(10.f, 10.f));
		_itemName->setSize(sf::Vector2f(2 * size.y - 10, 20.f));

		_itemDescription->setTextSize(sf::Vector2u(18u, 10u));
		_itemDescription->setPosition(sf::Vector2f(10.f, 20.f + _itemName->getCharacterSize()));
	}

	void ItemPreview::setParent(Widget* base) {
		base->add(_base);
	}

	void ItemPreview::minimalize() {
		
		_base->fullyDeactivate();
	}

	void ItemPreview::setPosition(sf::Vector2f pos) {
		_base->setPosition(pos);
		
	}

	void ItemPreview::setItem(EquipmentObject* item) {
		_base->fullyActivate();

		_itemDescription->setString(item->getDescription());
		_itemName->setString(item->getName());
		_itemImage->setTexture(item->getTexture());
	}

}