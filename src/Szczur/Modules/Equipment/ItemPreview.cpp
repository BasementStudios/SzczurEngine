#include "ItemPreview.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextAreaWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "EquipmentObject.hpp"
#include "EquipmentSlot.hpp"

namespace rat {
	ItemPreview::ItemPreview(sf::Texture* backgroundDescriptionText, sf::Font* font, sf::Font* italicFont)
	{
		_base = new Widget;
		_base->makeChildrenUnresizable();
		_base->setPropSize(.27f, 0.181f);
		_base->setPropPosition({ .5f, .05f });
		_base->makeChildrenPenetrable();

		_descriptionWidget = new ImageWidget;
		_descriptionWidget->setPropSize(.27f, 0.181f);
		_base->add(_descriptionWidget);
		_descriptionWidget->setTexture(backgroundDescriptionText);

		_itemName = new TextAreaWidget;
		_descriptionWidget->add(_itemName);
		_itemName->setFont(font);
		_itemName->setColor(sf::Color::White);
		_itemName->setCharacterPropSize(0.020f);
		_itemName->setPropSize(sf::Vector2f(.15f, 0.f));
		_itemName->setPropPosition(0.55f, 0.1f);
		_itemName->setAlign(TextAreaWidget::Align::Center);

		_itemDescription = new TextAreaWidget;
		_descriptionWidget->add(_itemDescription);
		_itemDescription->setFont(font);
		_itemDescription->setColor(sf::Color(66u, 244u, 119u));
		_itemDescription->setCharacterPropSize(0.015f);
		_itemDescription->setPropSize(sf::Vector2f(.23f, 0.f));
		_itemDescription->setPropPosition(.8f, 0.35f);
		_itemDescription->setAlign(TextAreaWidget::Align::Center);

		_itemLoreDescription = new TextAreaWidget;
		_descriptionWidget->add(_itemLoreDescription);
		_itemLoreDescription->setFont(italicFont);
		_itemLoreDescription->setColor(sf::Color::White);
		_itemLoreDescription->setCharacterPropSize(0.015f);
		_itemLoreDescription->setPropSize(sf::Vector2f(.23f, 0.f));
		_itemLoreDescription->setPropPosition(.8f, .75f);
		_itemLoreDescription->setAlign(TextAreaWidget::Align::Center);
	}

	void ItemPreview::setParent(Widget* base) {
		base->add(_base);
	}

	void ItemPreview::minimalize() {		
		_base->fullyDeactivate();
	}

	void ItemPreview::maximize() {
		_base->fullyActivate();
	}

	void ItemPreview::setItem(EquipmentObject* item, sf::Vector2f pos) {
		_base->setGlobalPosition(pos);

		_itemDescription->setString(item->getDescription());
		_itemName->setString(item->getName());
		_itemLoreDescription->setString(item->getLoreDescription());
	}

}