#include "ItemPreview.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextAreaWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "EquipmentObject.hpp"

namespace rat {
	ItemPreview::ItemPreview(sf::Texture* backgroundIconText, sf::Texture* backgroundDescriptionText, sf::Font* font)
	{
		_base = new Widget;
		_base->makeChildrenUnresizable();
		_base->setPropSize(.625f, 0.1f);
		_base->setPropPosition({ .5f, .2f });
		
		_descriptionWidget = new WindowWidget;
		_descriptionWidget->setPropSize(.625f, 0.1f);
		_base->add(_descriptionWidget);
		_descriptionWidget->setTexture(backgroundDescriptionText, 10);

		_itemName = new TextAreaWidget;
		_descriptionWidget->add(_itemName);
		_itemName->setFont(font);
		_itemName->setColor(sf::Color::White);
		_itemName->setCharacterSize(20);

		_itemDescription = new TextAreaWidget;
		_descriptionWidget->add(_itemDescription);
		_itemDescription->setFont(font);
		_itemDescription->setColor(sf::Color::White);
		_itemDescription->setCharacterSize(15);

		_itemName->setPosition(sf::Vector2f(10.f, 10.f));
		_itemName->setTextSize(sf::Vector2u(50u, 1u));

		_itemDescription->setTextSize(sf::Vector2u(50u, 2u));
		_itemDescription->setPosition(sf::Vector2f(10.f, 20.f + 15.f));
	}

	void ItemPreview::setParent(Widget* base) {
		base->add(_base);
	}

	void ItemPreview::minimalize() {		
		_base->fullyDeactivate();
	}

	void ItemPreview::setItem(EquipmentObject* item) {
		_base->fullyActivate();

		_itemDescription->setString(item->getDescription());
		_itemName->setString(item->getName());
	}

}