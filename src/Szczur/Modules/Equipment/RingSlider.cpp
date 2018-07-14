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
	RingSlider::RingSlider(Equipment* equipment, GUI& gui, sf::Vector2f frameSize)
		:_equipment(equipment)
	{
		_base = new Widget;

		/*_border = new WindowWidget;
		_border->setPropSize(0.15f, 0.6f);
		_base->add(_border);*/

		/*_scroll = new ScrollAreaWidget();
		_scroll->setPropSize(0.14f, 0.58f);
		_scroll->setPropPosition(0.5f, 0.5f);
		_base->add(_scroll);*/	
		
		_baseForItems = new ListWidget;
		_baseForItems->setPropBetweenPad(0.005f);
		_baseForItems->makeHorizontal();
		_base->add(_baseForItems);

		initAssetsViaGUI(gui);
		for (size_t i = 0; i < 2; i++)
		{
			_itemsLists.push_back(new ListWidget);
			_baseForItems->add(_itemsLists[i]);
			_itemsLists[i]->makeVertical();
			_itemsLists[i]->setPropBetweenPad(0.005f);
		}

		size_t row = 0;
		for (size_t i = 0; i < 10; i++)
		{
			if (i % 2 == 0)
				row = 0;
			else
				row = 1;
			EquipmentSlot* temp = new EquipmentSlot;
			_stoneSlots.push_back(temp);
			temp->setParent(_itemsLists[row]);
			temp->setPropSize(frameSize);
			temp->setTexture(_slotTexture, _shadowTexture);
			temp->getItemWidget()->setCallback(Widget::CallbackType::onHoverOut, [this, temp](Widget* owner) {
				if (temp->getItem())
				_equipment->disableItemPreview();
				temp->setHighlight(false);
			});
			temp->getItemWidget()->setCallback(Widget::CallbackType::onHoverIn, [this, temp](Widget* owner) {
				if(temp->getItem())
				_equipment->enableItemPreview(temp->getItem());
				temp->setHighlight(true);
			});
			temp->getWidget()->setCallback(Widget::CallbackType::onPress, [temp, this](Widget* owner) {
				if (temp->getItem())
				selectRing(temp);
			});
		}

	}

	void RingSlider::initAssetsViaGUI(GUI& gui) {
		//_border->setTexture(gui.getAsset<sf::Texture>("Assets/Equipment/ringsSlider.png"), 10);

		/*_scroll->setBoundsTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));
		_scroll->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
		_scroll->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));*/

		_slotTexture = gui.getAsset<sf::Texture>("Assets/Equipment/slot.png");
		_shadowTexture = gui.getAsset<sf::Texture>("Assets/Equipment/shadow.png");
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
		for (size_t i = 0; i < _stoneSlots.size(); i++)
		{
			if (!_stoneSlots[i]->getItem()) {
				_stoneSlots[i]->setItem(item);
				break;
			}
		}
	}

	void RingSlider::selectRing(EquipmentSlot* slot) {
		for (size_t i = 0; i < _selectedSlots.size(); i++)
		{
			if (_selectedSlots[i] == slot) {
				_equipment->_stoneStatusChanged(dynamic_cast<WearableItem*>(slot->getItem()), false);
				return;
			}
		}
		if (_selectedSlots.size() < 3) {
			_equipment->_stoneStatusChanged(dynamic_cast<WearableItem*>(slot->getItem()), true);
			slot->getItemWidget()->setColor(sf::Color(90u, 90u, 90u, 150u));
			dynamic_cast<WearableItem*>(slot->getItem())->activate();
			_selectedSlots.push_back(slot);
		}
	}

	std::vector<EquipmentObject*> RingSlider::getSelectedRings() {
		std::vector<EquipmentObject*> temp;
		for (auto& i : _selectedSlots)
		{
			temp.push_back(i->getItem());
		}
		return temp;
	}
	std::vector<EquipmentObject*> RingSlider::getStonesList() {
		std::vector<EquipmentObject*> temp;
		for (auto& i : _stoneSlots)
		{
			temp.push_back(i->getItem());
		}
		return temp;
	}
	
	void RingSlider::reset() {
		for (int i = _stoneSlots.size() - 1; i >= 0; i--)
		{
			if(_stoneSlots[i]->getItem())
			_stoneSlots[i]->removeItem();
		}
	}
	void RingSlider::_takeStoneOf(WearableItem* stone) {
		for (size_t i = 0; i < _selectedSlots.size(); i++)
		{
			if (dynamic_cast<WearableItem*>(_selectedSlots[i]->getItem()) == stone) {
				_selectedSlots[i]->resetItemColor();
				dynamic_cast<WearableItem*>(_selectedSlots[i]->getItem())->deactivate();
				_selectedSlots.erase(_selectedSlots.begin() + i);
			}
		}
	}
}