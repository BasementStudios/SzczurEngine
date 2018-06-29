#include "RingSlider.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Equipment.hpp"
#include "EquipmentSlot.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "WearableItem.hpp"

namespace rat {
	RingSlider::RingSlider(sf::Texture* frameText, sf::Vector2f frameSize, sf::Texture* pathText, sf::Texture* boundsText, sf::Texture* scrollText, Equipment* equipment, float length) 
		: _length(length), _equipment(equipment), _slotTexture(frameText), _slotSize(frameSize)
	{
		_base = new Widget;

		_scroll = new ScrollAreaWidget();		//bug: dodaje jakies kropki nw czemu
		_scroll->setPropSize(0.09f, 0.55f);
		_scroll->setBoundsTexture(boundsText);
		_scroll->setPathTexture(pathText);
		_scroll->setScrollerTexture(scrollText);
		_base->add(_scroll);

	}

	void RingSlider::setParent(Widget* newBase) {
		newBase->add(_base);
	}

	void RingSlider::setPosition(sf::Vector2f pos) {
		_base->setPosition(pos);
	}

	void RingSlider::addItem(WearableItem* item) {
		EquipmentSlot* temp = new EquipmentSlot;
		_ringSlots.push_back(temp);
		_scroll->resetScrollerPosition();
		temp->setParent(_scroll);
		temp->setSize(_slotSize);
		temp->setTexture(_slotTexture);
		temp->setPosition(sf::Vector2f(0.f, (_ringSlots.size() - 1) * (_slotSize.y + 10)));
		temp->setItem(item);
		temp->getItemWidget()->setCallback(Widget::CallbackType::onHoverOut, [this](Widget* owner) {
			this->_equipment->disableItemPreview();
		});
		temp->getItemWidget()->setCallback(Widget::CallbackType::onHoverIn, [this, item](Widget* owner) {
			this->_equipment->enableItemPreview(item);		
		});
		temp->getItemWidget()->setCallback(Widget::CallbackType::onPress, [temp, this](Widget* owner) {
			this->selectRing(temp);
		});
	}

	void RingSlider::selectRing(EquipmentSlot* slot) {
		for (size_t i = 0; i < _selectedSlots.size(); i++)
		{
			if (_selectedSlots[i] == slot) {
				slot->getItemWidget()->resetColor();
				dynamic_cast<WearableItem*>(slot->getItem())->deactivate();
				_selectedSlots.erase(_selectedSlots.begin() + i);
				return;
			}
		}
		if (_selectedSlots.size() < _maxSelectedRings) {
			slot->getItemWidget()->setColor(sf::Color::Color(119, 255, 89));
			dynamic_cast<WearableItem*>(slot->getItem())->activate();
			_selectedSlots.push_back(slot);
		}
	}
}