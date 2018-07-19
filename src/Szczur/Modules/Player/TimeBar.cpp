#include "TimeBar.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat {
	TimeBar::TimeBar() {
		_base = new Widget;
		_backgroundTime = new ImageWidget;
		_base->add(_backgroundTime);
		_foregroundTime = new ImageWidget;
		_base->add(_foregroundTime);
	}

	void TimeBar::setParent(Widget* newParent) {
		newParent->add(_base);
	}

	void TimeBar::initGUI(GUI& gui) {
		_foregroundTexture.push_back(gui.getTexture("Assets/Player/bars/time_bar/time_bar_1.png"));
		_foregroundTexture.push_back(gui.getTexture("Assets/Player/bars/time_bar/time_bar_2.png"));
		_foregroundTexture.push_back(gui.getTexture("Assets/Player/bars/time_bar/time_bar_3.png"));

		_backgroundTexture.push_back(gui.getTexture("Assets/Player/bars/time_bar/time_bar_back_1.png"));
		_backgroundTexture.push_back(gui.getTexture("Assets/Player/bars/time_bar/time_bar_back_2.png"));
		_backgroundTexture.push_back(gui.getTexture("Assets/Player/bars/time_bar/time_bar_back_3.png"));

		_fullBarTexture.push_back(gui.getTexture("Assets/Player/bars/time_bar/time_bar_full_1.png"));
		_fullBarTexture.push_back(gui.getTexture("Assets/Player/bars/time_bar/time_bar_full_2.png"));
		_fullBarTexture.push_back(gui.getTexture("Assets/Player/bars/time_bar/time_bar_full_3.png"));
	}

	void TimeBar::setTimeBarIndex(int index) {
		_backgroundTime->setTexture(_backgroundTexture[index]);
		_foregroundTime->setTexture(_foregroundTexture[index]);
		_currentBar = index;
		_recalcBar();
	}

	void TimeBar::setMaxTime(int newMaxTime) {
		_maxTime = newMaxTime;
		_recalcBar();
	}

	void TimeBar::setCurrentTime(int newCurrentTime) {
		_currentTime = newCurrentTime;
		_recalcBar();
	}

	void TimeBar::_recalcBar() {
		float percentOfBar = 0;
		if (_currentTime == _maxTime) {
			percentOfBar = 1.f;
			_foregroundTime->setTexture(_fullBarTexture[_currentBar]);
		}
		else if (_maxTime != 0) {
			if(_foregroundTime->getTexture() != _foregroundTexture[_currentBar])
				_foregroundTime->setTexture(_foregroundTexture[_currentBar]);
			if (_currentBar == 0)
				percentOfBar = _currentTime / _maxTime * 0.559f;
			if (_currentBar == 1)
				percentOfBar = _currentTime / _maxTime * 0.778f;
			if (_currentBar == 2)
				percentOfBar = _currentTime / _maxTime;
		}
		else
			percentOfBar = 0;
		_foregroundTime->setPropTextureRect(sf::FloatRect(0.f, 0.f, percentOfBar, 1.f));
		_foregroundTime->setPropSize({ _size.x * percentOfBar, _size.y });
		_foregroundTime->setPropPosition({ 0.f, 0.f });
	}

	void TimeBar::setPropSize(sf::Vector2f size) {
		_size = size;
		_base->setPropSize(size);
		_foregroundTime->setPropSize(size);
		_backgroundTime->setPropSize(size);
	}

	void TimeBar::setPropPosition(sf::Vector2f pos) {
		_base->setPropPosition(pos);
		_foregroundTime->setPropPosition(pos);
		_backgroundTime->setPropPosition(pos);
	}
}