#include "ReplaceItem.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextAreaWidget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "EquipmentObject.hpp"
#include "Equipment.hpp"

namespace rat {
	ReplaceItem::ReplaceItem(sf::Texture* backgroundIconText, sf::Texture* backgroundDescriptionText, sf::Texture* cancelText, sf::Font* font, Equipment* eq)
		:_equipment(eq)
	{
		_base = new Widget;
		_base->makeChildrenUnresizable();
		_base->setPropSize(.625f, 0.1f);
		_base->setPropPosition({ .5f, .03f });

		_descriptionWidget = new WindowWidget;
		_descriptionWidget->setPropSize(.625f, 0.1f);
		_base->add(_descriptionWidget);
		_descriptionWidget->setTexture(backgroundDescriptionText, 10);

		_itemName = new TextAreaWidget;
		_descriptionWidget->add(_itemName);
		_itemName->setFont(font);
		_itemName->setColor(sf::Color::White);
		_itemName->setCharacterPropSize(0.025f);
		_itemName->setPropPosition(sf::Vector2f(.7f, 0.1f));
		_itemName->setPropSize(sf::Vector2f(.3f, 0.1f));
		_itemName->setAlign(TextAreaWidget::Align::Right);

		_itemDescription = new TextAreaWidget;
		_descriptionWidget->add(_itemDescription);
		_itemDescription->setFont(font);
		_itemDescription->setColor(sf::Color::White);
		_itemDescription->setCharacterPropSize(0.019f);
		_itemDescription->setPropSize(sf::Vector2f(.3f, 0.f));
		_itemDescription->setPropPosition(sf::Vector2f(.7f, 0.4f));
		_itemDescription->setAlign(TextAreaWidget::Align::Right);

		_itemImage = new ImageWidget;
		_descriptionWidget->add(_itemImage);
		_itemImage->setPropSize(0.1f, 0.1f);
		_itemImage->setPropPosition(1.f, 0.5f);

		_cancelText = new TextAreaWidget;
		_descriptionWidget->add(_cancelText);
		_cancelText->setFont(font);
		_cancelText->setColor(sf::Color::White);
		_cancelText->setCharacterPropSize(0.02f);
		_cancelText->setPropPosition(0.05f, 0.1f);
		_cancelText->setPropSize(sf::Vector2f(.2f, 0.0f));
		_cancelText->setString("Wybierz przedmiot do wymiany");

		_cancelButton = new ImageWidget;
		_cancelButton->setTexture(cancelText);
		_descriptionWidget->add(_cancelButton);
		_cancelButton->setPropPosition(0.5f, 1.5f);
		_cancelButton->setCallback(Widget::CallbackType::onPress, [this](Widget* owner) {
			_equipment->_stopReplacingItem(false);
		});
		_cancelButton->setPropSize(0.13f,0.05f);
	}

	void ReplaceItem::setParent(Widget* base) {
		base->add(_base);
	}

	void ReplaceItem::close() {
		_base->fullyDeactivate();
		_isClosed = true;
	}

	void ReplaceItem::maximize() {
		if (!_isClosed)
			_base->fullyActivate();
	}

	void ReplaceItem::minimalize() {
		if (!_isClosed)
			_base->fullyDeactivate();
	}

	void ReplaceItem::setItem(EquipmentObject* item) {
		_base->fullyActivate();
		_isClosed = false;

		_itemImage->setTexture(item->getTexture());
		_itemDescription->setString(item->getDescription());
		_itemName->setString(item->getName());
	}
}