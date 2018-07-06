#include "RingSlider.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Equipment.hpp"
#include "EquipmentSlot.hpp"
#include "EquipmentObject.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "WearableItem.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"

namespace rat {
	RingSlider::RingSlider(sf::Vector2f frameSize, Equipment* equipment)
		:_equipment(equipment), _slotSize(frameSize)
	{
		_base = new Widget;

		_border = new WindowWidget;
		_border->setPropSize(0.15f, 0.6f);//(50.f + frameSize.x, 480.f);
		_base->add(_border);

		_scroll = new ScrollAreaWidget();
		//_scroll->setPropOrigin(.0f, .0f);
		//_scroll->setPropSize(0.09f, 0.65f);
		_scroll->setPropSize(0.14f, 0.58f);
		_scroll->setPropPosition(0.5f, 0.5f);
		_base->add(_scroll);	
		
		_baseForItems = new ListWidget;
		_baseForItems->setPropBetweenPad(0.005f);
		//_baseForItems->setPropSize({ 0.09f, 0.09f });
		_scroll->add(_baseForItems);

	}

	void RingSlider::initAssetsViaGUI(GUI& gui) {
		_border->setTexture(gui.getAsset<sf::Texture>("Assets/Equipment/ringsSlider.png"), 10);

		_scroll->setBoundsTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
		_scroll->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
		_scroll->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));

		_slotTexture = gui.getAsset<sf::Texture>("Assets/Equipment/slot.png");
		_slotChosenTexture = gui.getAsset<sf::Texture>("Assets/Equipment/chosenSlot.png");
		_highlightTexture = gui.getAsset<sf::Texture>("Assets/Equipment/highlight.png");
	}

	void RingSlider::setParent(Widget* newBase) {
		newBase->add(_base);
	}

	void RingSlider::setPropPosition(sf::Vector2f pos) {
		_base->setPropPosition(pos);
	}

	void RingSlider::setPosition(sf::Vector2f pos) {
		_base->setPosition(pos);
	}

	void RingSlider::addItem(WearableItem* item) {
		EquipmentSlot* temp = new EquipmentSlot;
		_ringSlots.push_back(temp);
		_scroll->resetScrollerPosition();
		//_baseForItems->setPropSize(0.09f, (_ringSlots.size() - 1) * (.09f + .01f));		
		temp->setParent(_baseForItems);
		temp->setPropSize({ 0.09f, 0.09f });//_slotSize);
		temp->setTexture(_slotTexture);
		//temp->setPropPosition(sf::Vector2f(0.f, (_ringSlots.size() - 1) * (.15f + .01f)));
		temp->setItem(item);
		temp->setHighlightTexture(_highlightTexture);
		temp->getItemWidget()->setCallback(Widget::CallbackType::onHoverOut, [this, temp](Widget* owner) {
			this->_equipment->disableItemPreview();
			temp->setHighlight(false);
		});
		temp->getItemWidget()->setCallback(Widget::CallbackType::onHoverIn, [this, item, temp](Widget* owner) {
			this->_equipment->enableItemPreview(item);		
			temp->setHighlight(true);
		});
		temp->getWidget()->setCallback(Widget::CallbackType::onPress, [temp, this](Widget* owner) {
			this->selectRing(temp);
		});
	}

	void RingSlider::selectRing(EquipmentSlot* slot) {
		for (size_t i = 0; i < _selectedSlots.size(); i++)
		{
			if (_selectedSlots[i] == slot) {
				slot->setTexture(_slotTexture);
				dynamic_cast<WearableItem*>(slot->getItem())->deactivate();
				_selectedSlots.erase(_selectedSlots.begin() + i);
				return;
			}
		}
		if (_selectedSlots.size() < _maxSelectedRings) {
			slot->setTexture(_slotChosenTexture);
			dynamic_cast<WearableItem*>(slot->getItem())->activate();
			_selectedSlots.push_back(slot);
		}
	}

	void RingSlider::setSelectedRingsLimit(int newSize) {
		_maxSelectedRings = newSize;
	}

	std::vector<EquipmentObject*> RingSlider::getSelectedRings() {
		std::vector<EquipmentObject*> temp;
		for (auto& i : _selectedSlots)
		{
			temp.push_back(i->getItem());
		}
		return temp;
	}
	std::vector<EquipmentObject*> RingSlider::getRingsList() {
		std::vector<EquipmentObject*> temp;
		for (auto& i : _ringSlots)
		{
			temp.push_back(i->getItem());
		}
		return temp;
	}
}