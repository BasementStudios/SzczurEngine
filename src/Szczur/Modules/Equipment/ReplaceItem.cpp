#include "ReplaceItem.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextAreaWidget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "EquipmentObject.hpp"
#include "NormalSlots.hpp"

namespace rat {
	ReplaceItem::ReplaceItem(sf::Texture* backgroundIconText, sf::Texture* backgroundDescriptionText, sf::Texture* cancelText, sf::Font* font, NormalSlots* slots)
		:_slots(slots)
	{
		_base = new Widget;
		_base->makeChildrenUnresizable();
		_base->setPropSize(.625f, 0.1f);
		_base->setPropPosition({ .5f, .05f });

		_descriptionWidget = new WindowWidget;
		_descriptionWidget->setPropSize(.625f, 0.1f);
		_base->add(_descriptionWidget);
		_descriptionWidget->setTexture(backgroundDescriptionText, 10);

		_itemName = new TextAreaWidget;
		_descriptionWidget->add(_itemName);
		_itemName->setFont(font);
		_itemName->setColor(sf::Color::White);
		_itemName->setCharacterSize(20);
		_itemName->setPosition(sf::Vector2f(10.f, 10.f));
		_itemName->setTextSize(sf::Vector2u(50u, 1u));

		_itemDescription = new TextAreaWidget;
		_descriptionWidget->add(_itemDescription);
		_itemDescription->setFont(font);
		_itemDescription->setColor(sf::Color::White);
		_itemDescription->setCharacterSize(15);
		_itemDescription->setTextSize(sf::Vector2u(50u, 2u));
		_itemDescription->setPosition(sf::Vector2f(10.f, 20.f + 15.f));

		_itemImage = new ImageWidget;
		_descriptionWidget->add(_itemImage);
		_itemImage->setPropSize(0.1f, 0.1f);
		_itemImage->setPropPosition(1.f, 0.5f);

		_cancelText = new TextWidget;
		_descriptionWidget->add(_cancelText);
		_cancelText->setFont(font);
		_cancelText->setColor(sf::Color::White);
		_cancelText->setCharacterSize(20);
		_cancelText->setPropPosition(0.5f, 0.f);
		_cancelText->setString("Wybierz przedmiot do wymiany");

		_cancelButton = new ImageWidget;
		_cancelButton->setTexture(cancelText);
		_descriptionWidget->add(_cancelButton);
		_cancelButton->setPropPosition(0.5f, 1.0f);
		_cancelButton->setCallback(Widget::CallbackType::onPress, [this](Widget* owner) {
			_base->fullyDeactivate();
			_slots->_stopReplacing();
		});
	}

	void ReplaceItem::setParent(Widget* base) {
		base->add(_base);
	}

	void ReplaceItem::minimalize() {
		_base->fullyDeactivate();
	}

	void ReplaceItem::setItem(EquipmentObject* item) {
		_base->fullyActivate();

		_itemImage->setTexture(item->getTexture());
		_itemDescription->setString(item->getDescription());
		_itemName->setString(item->getName());
	}
}