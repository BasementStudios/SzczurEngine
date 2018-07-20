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
		_highlightPPTexture = gui.getTexture("Assets/Player/pp_light.png");;
	}

	void PPCount::setHighlightedPPAmount(int newAmount) {
		if (newAmount != _highlightedPP) {
			for (int i = _PPCount - 1; i >= 0; i--)
			{
				if (_PPCount > 0 && i < _PPCount - newAmount) {
					_PPList[i]->setTexture(_PPTexture);
				}
				else if (_PPCount > 0 && i < _PPCount && i >= _PPCount - newAmount) {
					_PPList[i]->setTexture(_highlightPPTexture);
				}

			}
		}
		_highlightedPP = newAmount;
	}

	void PPCount::setSlotsAmount(int goodAmount, int badAmount) {
		if (_goodCount != goodAmount || _brokenCount != badAmount) {
			_goodCount = goodAmount;
			_brokenCount = badAmount;
			for (size_t i = 0; i < _slotCount; i++)
			{
				if (i < _goodCount) {
					_slotList[i]->setTexture(_slotTexture);
				}
				else if (i >= _goodCount && i < _goodCount + _brokenCount) {
					_slotList[i]->setTexture(_brokenSlotTexture);
				}
				else {
					_slotList[i]->fullyDeactivate();
				}
			}
		}
	}


	void PPCount::setGoodSlotAmount(int newAmount) {
		if (_goodCount != newAmount) {
			_goodCount = newAmount;
			for (size_t i = 0; i < _slotCount; i++)
			{
				if (i < newAmount) {
					_slotList[i]->setTexture(_slotTexture);
				}
				else if (i >= newAmount && i < newAmount + _brokenCount){
					_slotList[i]->setTexture(_brokenSlotTexture);
				}
				else {
					_slotList[i]->fullyDeactivate();
				}
			}		
		}	
	}

	void PPCount::setBrokenSlotAmount(int newAmount) {
		if (_brokenCount != newAmount) {
			for (size_t i = 0; i < _slotCount; i++)
			{
				if (i < _goodCount)
				{
					_slotList[i]->setTexture(_slotTexture);
				}
				if (i >= _goodCount && i < newAmount + _goodCount) {
					_slotList[i]->setTexture(_brokenSlotTexture);
				}
				else if(!i < newAmount + _goodCount) {
					_slotList[i]->fullyDeactivate();
				}
			}
			_brokenCount = newAmount;
		}
	}

	void PPCount::setPPCount(int newAmount) {
		if (_PPCount != newAmount) {
			_PPCount = newAmount;
			for (int i = _slotCount - 1; i >= 0; i--)
			{
				if (i < newAmount) {  
					if (i >= _PPCount - _highlightedPP) {
						_PPList[i]->setTexture(_highlightPPTexture);
					}
					else {
						_PPList[i]->setTexture(_PPTexture);
					}
				}
				else {
					_PPList[i]->removeTexture();
				}
			}      
		}
	}
}