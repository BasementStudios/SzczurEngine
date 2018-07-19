#include "Player.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

#include "Szczur/Modules/GUI/InterfaceWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

#include "HPBar.hpp"
#include "TimeBar.hpp"
#include "PPCount.hpp"
#include "SkillSlots.hpp"
#include "ItemSlots.hpp"

namespace rat {
	Player::Player() {
		LOG_INFO("Initializing Player module");
		_pathToJson = "Assets/Player/skills.json";
	
		initScript();
		initJson();
		initGUI();

		LOG_INFO("Player module initialized");
	}

	Player::~Player() {
		LOG_INFO("Player module destructed");
	}

	void Player::initGUI() {
		auto& gui = getModule<GUI>();

		gui.addTexture("Assets/Player/bars/hp_bar/hp_bar_1.png");
		gui.addTexture("Assets/Player/bars/hp_bar/hp_bar_2.png");
		gui.addTexture("Assets/Player/bars/hp_bar/hp_bar_3.png");

		gui.addTexture("Assets/Player/bars/hp_bar/hp_bar_back_1.png");
		gui.addTexture("Assets/Player/bars/hp_bar/hp_bar_back_2.png");
		gui.addTexture("Assets/Player/bars/hp_bar/hp_bar_back_3.png");

		gui.addTexture("Assets/Player/bars/time_bar/time_bar_1.png");
		gui.addTexture("Assets/Player/bars/time_bar/time_bar_2.png");
		gui.addTexture("Assets/Player/bars/time_bar/time_bar_3.png");

		gui.addTexture("Assets/Player/bars/time_bar/time_bar_back_1.png");
		gui.addTexture("Assets/Player/bars/time_bar/time_bar_back_2.png");
		gui.addTexture("Assets/Player/bars/time_bar/time_bar_back_3.png");

		gui.addTexture("Assets/Player/bars/time_bar/time_bar_full_1.png");
		gui.addTexture("Assets/Player/bars/time_bar/time_bar_full_2.png");
		gui.addTexture("Assets/Player/bars/time_bar/time_bar_full_3.png");

		gui.addTexture("Assets/Player/pp.png");
		gui.addTexture("Assets/Player/pp_back.png");
		gui.addTexture("Assets/Player/pp_broken.png");

		gui.addTexture("Assets/Player/skills/skill_back.png");
		gui.addTexture("Assets/Player/skills/selected_skill_back.png");

		gui.addTexture("Assets/Player/1pp.png");
		gui.addTexture("Assets/Player/2pp.png");
		gui.addTexture("Assets/Player/3pp.png");
		gui.addTexture("Assets/Player/4pp.png");
		gui.addFont("Assets/Player/SourceSansPro-SemiBold.ttf");

		gui.addTexture("Assets/Player/item_slot.png");

		gui.addTexture("Assets/Player/background.png");
		gui.addTexture("Assets/Player/gui_back.png");

		_base = gui.addInterface();
		_base->setSizingWidthToHeightProportion(1.f);

		_background = new ImageWidget;
		_base->add(_background);
		_background->setTexture(gui.getTexture("Assets/Player/background.png"));
		_background->setPropSize({1.77f, 1.f});

		_HPBack = new ImageWidget;
		_base->add(_HPBack);
		_HPBack->setTexture(gui.getTexture("Assets/Player/gui_back.png"));
		_HPBack->setPropPosition(0.f, 0.f);
		_HPBack->setPropSize(0.37f, 0.08f);

		_HPBar = new HPBar();
		_HPBar->setParent(_base);
		_HPBar->initGUI(gui);
		_HPBar->setPropSize({0.46f, 0.125f});
		_HPBar->setPropPosition({0.03f, 0.01f});	
		_HPBar->setMaxHP(100);
		_HPBar->setCurrentHP(95);
		_HPBar->setHPBarIndex(1);

		_TimeBar = new TimeBar();
		_TimeBar->setParent(_base);
		_TimeBar->initGUI(gui);
		_TimeBar->setPropSize({ 0.384f, 0.057f });
		_TimeBar->setPropPosition({ 0.09f, 0.073f });
		_TimeBar->setMaxTime(100);
		_TimeBar->setCurrentTime(95);
		_TimeBar->setTimeBarIndex(1);

		_PPBack = new ImageWidget;
		_base->add(_PPBack);
		_PPBack->setTexture(gui.getTexture("Assets/Player/gui_back.png"));
		_PPBack->setPropPosition(0.5f, 0.f);
		_PPBack->setPropSize(0.6f, 0.1f);

		_PPCount = new PPCount;
		_PPCount->setParent(_base);
		_PPCount->initGUI(gui);
		_PPCount->setPropPosition({0.5f, 0.f});
		_PPCount->setBrokenSlotAmount(10);
		_PPCount->setPPCount(5);

		_skillSlots = new SkillSlots(gui);
		_skillSlots->setParent(_base);
		_skillSlots->setPropPosition({0.5f, 0.05f});
		_skillSlots->addSkill(&_skillsList[0]);
		_skillSlots->addSkill(&_skillsList[1]);

		_itemBack = new ImageWidget;
		_base->add(_itemBack);
		_itemBack->setTexture(gui.getTexture("Assets/Player/gui_back.png"));
		_itemBack->setPropPosition(1.f, 0.f);
		_itemBack->setPropSize(0.4f, 0.08f);

		_itemSlots = new ItemSlots(gui);
		_itemSlots->setParent(_base);
		_itemSlots->setPropPosition({ .97f, 0.03f });
	}

	void Player::initJson() {
		_skillsList.clear();

		nlohmann::json config;

		std::ifstream file(_pathToJson);

		if (file.good()) {
			file >> config;
			if (auto& skills = config["skills"]; !skills.is_null()) {
				for (auto itr = config["skills"].begin(); itr != config["skills"].end(); ++itr) {
					auto& skill = itr.value();

					Skill temp;
					temp.setNameID(itr.key());

					if (auto& name = skill["name"]; !skill["name"].is_null()) {
						temp.setName(name);
					}

					if (auto& description = skill["description"]; !skill["description"].is_null()) {
						temp.setDescription(description);
					}

					if (auto& PPCost = skill["PPCost"]; !skill["PPCost"].is_null()) {
						temp.setPPCost(PPCost);
					}

					if (auto& essenceCost = skill["essenceCost"]; !skill["essenceCost"].is_null()) {
						for (size_t i = 0; i < 4; i++)
						{
							temp.setEssenceCost(i, essenceCost[i].get<int>());
						}			
					}

					if (auto& type = skill["type"]; !skill["type"].is_null()) {
						temp.setSkillType(type);
					}

					if (auto& icon = skill["iconPath"]; !skill["iconPath"].is_null()) {
						temp.setIcon(icon);
					}

					_skillsList.push_back(temp);
				}
			}
		}
	}

	void Player::initScript() {
		Script& script = getModule<Script>();

		auto module = script.newModule("Player");

		module.set_function("getHP", &Player::getHP, this);
		module.set_function("setHP", &Player::setHP, this);
		module.set_function("addHP", &Player::addHP, this);

		module.set_function("setMaxHP", &Player::setMaxHP, this);
		module.set_function("getMaxHP", &Player::getMaxHP, this);

		module.set_function("getPP", &Player::getPP, this);
		module.set_function("setPP", &Player::setPP, this);
		module.set_function("addPP", &Player::addPP, this);

		module.set_function("setMaxPP", &Player::setMaxPP, this);
		module.set_function("getMaxPP", &Player::getMaxPP, this);

		module.set_function("getSkill", &Player::getSkill, this);
		module.set_function("addSkill", &Player::addSkill, this);
		module.set_function("removeSkill", &Player::removeSkill, this);

		script.initClasses<Skill>();
	}

	const std::string& Player::getPathToJson() {
		return _pathToJson;
	}

	void Player::reloadJson() {
		initJson();
	}

	bool Player::addSkill(const std::string& nameID) {
		for (auto& i : _skillsList) {
			if (i.getNameID() == nameID) {
				i.setIsKnown(true);
				return true;
			}
		}
		return false;
	}

	bool Player::removeSkill(const std::string& nameID) {
		for (std::size_t i = 0; i < _skillsList.size(); i++)
		{
			if (_skillsList[i].getNameID() == nameID && _skillsList[i].getIsKnown()) {
				_skillsList[i].setIsKnown(false);
				return true;
			}
		}
		return false;
	}

	const Skill& Player::getSkill(const std::string& nameID) {
		for (auto& i : _skillsList) {
			if (i.getNameID() == nameID) {
				return i;
			}
		}
	}

	//HP section
	int Player::getHP() {
		return _currentHP;
	}

	void Player::addHP(int HP) {
		_currentHP += HP;
		_currentHP = std::clamp(_currentHP, 0, _maxHP);
		_HPBar->setCurrentHP(_currentHP);
	}

	void Player::setHP(int newHP) {
		_currentHP = newHP;
		_currentHP = std::clamp(_currentHP, 0, _maxHP);
		_HPBar->setCurrentHP(_currentHP);
	}

	int Player::getMaxHP() {
		return _maxHP;
	}

	void Player::setMaxHP(int newMaxHP) {
		_maxHP = newMaxHP;
		_HPBar->setMaxHP(_maxHP);
	}

	//PP section
	int Player::getPP() {
		return _currentPP;
	}

	void Player::addPP(int PP) {
		_currentPP += PP;
		_currentPP = std::clamp(_currentPP, 0, _maxPP);
		_PPCount->setPPCount(_currentPP);
	}

	void Player::setPP(int PP) {
		_currentPP = PP;
		_currentPP = std::clamp(_currentPP, 0, _maxPP);
		_PPCount->setPPCount(_currentPP);
	}

	int Player::getMaxPP() {
		return _maxPP;
	}

	void Player::setMaxPP(int newMaxPP) {
		_maxPP = newMaxPP;
	}
	
	void Player::setHPBarIndex(int i) {
		_HPBar->setHPBarIndex(i);
	}

	void Player::setTimeBarIndex(int i) {
		_TimeBar->setTimeBarIndex(i);
	}
	void Player::setCurrentTime(int time) {
		_TimeBar->setCurrentTime(time);
	}
	void Player::setMaxTime(int time) {
		_TimeBar->setMaxTime(time);
	}

	void Player::setBrokenPPAmount(int amount) {
		_PPCount->setBrokenSlotAmount(amount);
	}
	void Player::setGoodPPAmount(int amount) {
		_PPCount->setGoodSlotAmount(amount);
	}

	void Player::addSkillToSlot(const std::string& nameID) {
		for (size_t i = 0; i < _skillsList.size(); i++)
		{
			if (nameID == _skillsList[i].getNameID()) {
				_skillSlots->addSkill(&_skillsList[i]);
			}
		}
	}
	void Player::chooseSkill(const std::string& nameID) {
		_skillSlots->chooseSkill(nameID);
	}
	void Player::unChooseSkill(const std::string& nameID) {
		_skillSlots->unChooseSkill(nameID);
	}
	void Player::setCounter(const std::string& nameID, const std::string& number) {
		//if number = "0" skill resets (number is no longer visible)
		_skillSlots->setCounter(nameID, number);
	}

	void Player::addItem(const std::string& nameID, sf::Texture* text) {
		_itemSlots->setItem(text, nameID);
	}
	void Player::removeItem(const std::string& nameID) {
		_itemSlots->removeItem(nameID);
	}
}