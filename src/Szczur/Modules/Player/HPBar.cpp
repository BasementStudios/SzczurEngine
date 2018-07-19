#include "HPBar.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat {
	HPBar::HPBar() {
		_base = new Widget;
		_backgroundHeart = new ImageWidget;
		_base->add(_backgroundHeart);
		_foregroundHeart = new ImageWidget;
		_base->add(_foregroundHeart);

		_poisoningStatus = new ImageWidget;
		_base->add(_poisoningStatus);
		_poisoningStatus->setPropPosition(0.4f, 1.3f);
		_poisoningStatus->fullyDeactivate();

		_bleedingStatus = new ImageWidget;
		_base->add(_bleedingStatus);
		_bleedingStatus->setPropPosition(0.2f, 1.3f);
		_bleedingStatus->fullyDeactivate();

	}

	void HPBar::setParent(Widget* newParent) {
		newParent->add(_base);
	}

	void HPBar::initGUI(GUI& gui) {
		_foregroundTexture.push_back(gui.getTexture("Assets/Player/bars/hp_bar/hp_bar_1.png"));
		_foregroundTexture.push_back(gui.getTexture("Assets/Player/bars/hp_bar/hp_bar_2.png"));
		_foregroundTexture.push_back(gui.getTexture("Assets/Player/bars/hp_bar/hp_bar_3.png"));

		_backgroundTexture.push_back(gui.getTexture("Assets/Player/bars/hp_bar/hp_bar_back_1.png"));
		_backgroundTexture.push_back(gui.getTexture("Assets/Player/bars/hp_bar/hp_bar_back_2.png"));
		_backgroundTexture.push_back(gui.getTexture("Assets/Player/bars/hp_bar/hp_bar_back_3.png"));

		_bleedingStatus->setTexture(gui.getTexture("Assets/Player/bleeding.png"));
		_poisoningStatus->setTexture(gui.getTexture("Assets/Player/poisoning.png"));
	}

	void HPBar::setHPBarIndex(int index) {
		_backgroundHeart->setTexture(_backgroundTexture[index]);
		_foregroundHeart->setTexture(_foregroundTexture[index]);
		_currentBar = index;
		_recalcBar();
	}

	void HPBar::setMaxHP(int newMaxHP) {
		_maxHP = newMaxHP;
		_recalcBar();
	}

	void HPBar::setCurrentHP(int newCurrentHP) {
		_currentHP = newCurrentHP;
		_recalcBar();
	}

	void HPBar::_recalcBar() {
		float percentOfBar = 0;
		if (_maxHP != 0) {
			if (_currentBar == 0)
				percentOfBar = _currentHP / _maxHP * 0.68f;
			if (_currentBar == 1)
				percentOfBar = _currentHP / _maxHP * 0.85f;
			if (_currentBar == 2)
				percentOfBar = _currentHP / _maxHP;
		}
		else
			percentOfBar = 0;
		_foregroundHeart->setPropTextureRect(sf::FloatRect(0.f, 0.f, percentOfBar, 1.f));
		_foregroundHeart->setPropSize({_size.x * percentOfBar, _size.y});
		_foregroundHeart->setPropPosition({0.f, 0.f});
	}

	void HPBar::setPropSize(sf::Vector2f size) {
		_size = size;
		_base->setPropSize(size);
		_foregroundHeart->setPropSize(size);
		_backgroundHeart->setPropSize(size);
		_poisoningStatus->setPropSize(size.y / 3, size.y / 3);
		_bleedingStatus->setPropSize(size.y / 3, size.y / 3);
	}

	void HPBar::setPropPosition(sf::Vector2f pos) {
		_base->setPropPosition(pos);
		_foregroundHeart->setPropPosition(pos);
		_backgroundHeart->setPropPosition(pos);
	}

	void HPBar::setStatus(const std::string& name) {
		if (name == "bleeding") {
			_bleedingStatus->fullyActivate();
			_isBleedingActivated = true;
		}
		if (name == "poisoning") {
			_poisoningStatus->fullyActivate();
			_isPoisoningActivated = true;
			if(_isBleedingActivated)
				_poisoningStatus->setPropPosition(0.32f, 1.3f);
			else
				_poisoningStatus->setPropPosition(0.2f, 1.3f);
		}
	}
	void HPBar::removeStatus(const std::string& name) {
		if (name == "bleeding") {
			_bleedingStatus->fullyDeactivate();
			_isBleedingActivated = false;
			if(_isPoisoningActivated)
				_poisoningStatus->setPropPosition(0.2f, 1.3f);
		}
		if (name == "poisoning") {
			_poisoningStatus->fullyDeactivate();
			_isPoisoningActivated = false;
		}
	}
}