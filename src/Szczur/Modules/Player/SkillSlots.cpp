#include "SkillSlots.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"
#include "Szczur/Modules/GUI/TextAreaWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Skill.hpp"

namespace rat {
	SkillSlots::SkillSlots(GUI& gui) {
		_base = new Widget;

		_slotsListWidget = new ListWidget;
		_base->add(_slotsListWidget);
		_slotsListWidget->setPropBetweenPad(0.005f);
		_slotsListWidget->makeHorizontal();

		initGUI(gui);

		for (size_t i = 0; i < 6; i++)
		{
			_slotsList.push_back(new SkillSlot);
			_slotsList[i]->slotImage = new ImageWidget;
			_slotsList[i]->slotImage->setPropSize(0.09f, 0.09f);
			_slotsList[i]->slotImage->setTexture(_slotTexture);
			_slotsListWidget->add(_slotsList[i]->slotImage);

			_slotsList[i]->skillImage = new ImageWidget;
			_slotsList[i]->skillImage->setPropSize(0.092f, 0.092f);
			_slotsList[i]->slotImage->add(_slotsList[i]->skillImage);
			_slotsList[i]->skillImage->setPropPosition(0.5f, 0.5f);

			_slotsList[i]->PPImage = new ImageWidget;
			_slotsList[i]->PPImage->setPropSize(0.073f, 0.02f);
			_slotsList[i]->slotImage->add(_slotsList[i]->PPImage);
			_slotsList[i]->PPImage->setPropPosition(0.45f, 1.2f);

			_slotsList[i]->itemCounter = new TextAreaWidget;
			_slotsList[i]->itemCounter->setPropSize(0.073f, 0.0f);
			_slotsList[i]->slotImage->add(_slotsList[i]->itemCounter);
			_slotsList[i]->itemCounter->setPropPosition(0.4f, .2f);
			_slotsList[i]->itemCounter->setFont(_font);
			_slotsList[i]->itemCounter->setCharacterPropSize(0.06f);
			_slotsList[i]->itemCounter->setColor(sf::Color::White);
			_slotsList[i]->itemCounter->setAlign(TextAreaWidget::Align::Center);
		}
	}

	void SkillSlots::setParent(Widget* base) {
		base->add(_base);
	}

	void SkillSlots::initGUI(GUI& gui){
		_slotTexture = gui.getTexture("Assets/Player/skills/skill_back.png");

		_chosenSlotTexture = gui.getTexture("Assets/Player/skills/selected_skill_back.png");

		_PPList.push_back(gui.getTexture("Assets/Player/1pp.png"));
		_PPList.push_back(gui.getTexture("Assets/Player/2pp.png"));
		_PPList.push_back(gui.getTexture("Assets/Player/3pp.png"));
		_PPList.push_back(gui.getTexture("Assets/Player/4pp.png"));

		_font = gui.getFont("Assets/Player/SourceSansPro-SemiBold.ttf");
	}

	void SkillSlots::addSkill(Skill* skill) {
		for (size_t i = 0; i < 6; i++)
		{
			if (!_slotsList[i]->skill) {
				_slotsList[i]->skill = skill;
				_slotsList[i]->skillImage->setTexture(skill->getIcon());

				_slotsList[i]->PPImage->setTexture(_PPList[skill->getPPcost() - 1]);
				break;
			}
		}
	}

	void SkillSlots::setPropPosition(sf::Vector2f pos) {
		_base->setPropPosition(pos);
	}

	void SkillSlots::chooseSkill(const std::string& nameID) {
		for (size_t i = 0; i < 6; i++)
		{
			if (_slotsList[i]->isChosen && _slotsList[i]->skill && _slotsList[i]->skill->getNameID() != nameID) {
				_slotsList[i]->slotImage->setTexture(_slotTexture);
				_slotsList[i]->isChosen = false;
			}
			else if (_slotsList[i]->skill && _slotsList[i]->skill->getNameID() == nameID) {
				_slotsList[i]->slotImage->setTexture(_chosenSlotTexture);
				_slotsList[i]->isChosen = true;
			}
		}
		_isSomeSlotChosen = true;
	}

	void SkillSlots::unChooseSkill(const std::string& nameID) {
		for (size_t i = 0; i < 6; i++)
		{
			if (_slotsList[i]->skill && _slotsList[i]->skill->getNameID() == nameID) {
				_slotsList[i]->slotImage->setTexture(_slotTexture);
				_slotsList[i]->isChosen = false;
			}
		}
	}

	void SkillSlots::setCounter(const std::string& nameID, const std::string& number) {
		for (size_t i = 0; i < 6; i++)
		{
			if (_slotsList[i]->skill && _slotsList[i]->skill->getNameID() == nameID) {
				if (number == "0") {
					_slotsList[i]->itemCounter->fullyDeactivate();
					_slotsList[i]->skillImage->resetColor();
				}
				else {
				_slotsList[i]->itemCounter->setString(number);
				_slotsList[i]->skillImage->setColor(sf::Color(100u, 100u, 100u));
				_slotsList[i]->itemCounter->fullyActivate();
				}
			}
		}
	}


}