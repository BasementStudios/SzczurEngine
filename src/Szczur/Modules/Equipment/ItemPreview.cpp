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
		_base->setPropPosition({ .5f, .17f });
		
		_descriptionWidget = new WindowWidget;
		_descriptionWidget->setPropSize(.625f, 0.1f);
		_base->add(_descriptionWidget);
		_descriptionWidget->setTexture(backgroundDescriptionText, 10);

		_itemName = new TextAreaWidget;
		_descriptionWidget->add(_itemName);
		_itemName->setFont(font);
		_itemName->setColor(sf::Color::White);
		_itemName->setCharacterPropSize(0.025f);

		_itemDescription = new TextAreaWidget;
		_descriptionWidget->add(_itemDescription);
		_itemDescription->setFont(font);
		_itemDescription->setColor(sf::Color::White);
		_itemDescription->setCharacterPropSize(0.019f);

		_itemName->setPropPosition(0.1f, 0.1f);
		_itemName->setPropSize(sf::Vector2f(.5f, 0.f));

		_itemDescription->setPropSize(sf::Vector2f(.5f, 0.f));
		_itemDescription->setPropPosition(0.1f, 0.5f);
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