#include "PPCount.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat {
	PPCount::PPCount() {
		_base = new Widget;

		_slotListWidget = new ListWidget;
		_base->add(_slotListWidget);
		_slotListWidget->setPropBetweenPad(0.003f);
		_slotListWidget->makeHorizontal();

		_PPListWidget = new ListWidget;
		_base->add(_PPListWidget);
		_PPListWidget->setPropBetweenPad(0.003f);
		_PPListWidget->makeHorizontal();

		for (size_t i = 0; i < _slotCount; i++)
		{
			_slotList.push_back(new ImageWidget);
			_slotListWidget->add(_slotList[i]);
			_slotList[i]->setPropSize(0.04f, 0.04f);

			_PPList.push_back(new ImageWidget);
			_PPListWidget->add(_PPList[i]);
			_PPList[i]->setPropSize(0.04f, 0.04f);
		}
	}

	void PPCount::setParent(Widget* base) {
		base->add(_base);
	}

	void PPCount::setPropPosition(sf::Vector2f pos) {
		_base->setPropPosition(pos);
	}

	void PPCount::setPropSize(sf::Vector2f size) {
		_base->setPropSize(size);
	}

	void PPCount::initGUI(GUI& gui){
		_PPTexture = gui.getTexture("Assets/Player/pp.png");
		_slotTexture = gui.getTexture("Assets/Player/pp_back.png");
		_brokenSlotTexture = gui.getTexture("Assets/Player/pp_broken.png");
	}

	void PPCount::setGoodSlotAmount(int newAmount) {
		if (15 - _brokenCount != newAmount) {
			for (size_t i = 0; i < _slotCount; i++)
			{
				if (i < newAmount) {
					_slotList[i]->setTexture(_slotTexture);
				}
				else {
					_slotList[i]->setTexture(_brokenSlotTexture);
				}
			}
			_brokenCount = 15 - newAmount;
		}	
	}

	void PPCount::setBrokenSlotAmount(int newAmount) {
		if (_brokenCount != newAmount) {
			for (size_t i = 0; i < _slotCount; i++)
			{
				if (i < _slotCount - newAmount) {
					_slotList[i]->setTexture(_slotTexture);
				}
				else {
					_slotList[i]->setTexture(_brokenSlotTexture);
				}
			}
			_brokenCount = newAmount;
		}
	}

	void PPCount::setPPCount(int newAmount) {
		if (_PPCount != newAmount) {
			for (size_t i = 0; i < _slotCount; i++)
			{
				if (i < newAmount) {
					_PPList[i]->setTexture(_PPTexture);
				}
				else {
					_PPList[i]->removeTexture();
				}
			}
			_PPCount = newAmount;
		}
	}
}