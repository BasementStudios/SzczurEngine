#include "Player.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

namespace rat {
	//skill methods
	void Skill::initScript(Script& script) {
		auto object = script.newClass<Skill>("Skill", "Player");
		object.set("getName", &Skill::getName);
		object.set("getDescription", &Skill::getDescription);
		object.set("getPPCost", &Skill::getPPcost);
		object.set("getEssenceCost", &Skill::getEssenceCost);
		object.set("getSkillType", &Skill::getSkillType);
		object.init();
	}

	void Skill::setNameID(const std::string& _nameID) {
		nameID = _nameID;
	}
	const std::string& Skill::getNameID() {
		return nameID;
	}

	void Skill::setName(const std::string& _name) {
		name = _name;
	}
	const std::string& Skill::getName() {
		return name;
	}

	void Skill::setDescription(const std::string& _description) {
		description = _description;
	}
	const std::string& Skill::getDescription() {
		return description;
	}

	void Skill::setPPCost(int cost) {
		PPCost = cost;
	}
	int Skill::getPPcost() {
		return PPCost;
	}

	void Skill::setEssenceCost(size_t index, int value) {
		essenceContainer[index] = value;
	}
	int Skill::getEssenceCost(size_t index) {
		return essenceContainer[index];
	}

	void Skill::setSkillType(int path) {
		type = path;
	}
	int Skill::getSkillType() {
		return type;
	}

	Player::Player() {
		LOG_INFO("Initializing Player module");
		_pathToJson = "Assets/Skills/skills.json";

		initScript();
		initJson();

		LOG_INFO("Player module initialized");
	}

	Player::~Player() {
		LOG_INFO("Player module destructed");
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

					Skill* temp = new Skill;
					temp->setNameID(itr.key());

					if (auto& name = skill["name"]; !skill["name"].is_null()) {
						temp->setName(name);
					}

					if (auto& description = skill["description"]; !skill["description"].is_null()) {
						temp->setDescription(description);
					}

					if (auto& PPCost = skill["PPCost"]; !skill["PPCost"].is_null()) {
						temp->setPPCost(PPCost);
					}

					if (auto& essenceCost = skill["essenceCost"]; !skill["essenceCost"].is_null()) {
						for (size_t i = 0; i < 4; i++)
						{
							temp->setEssenceCost(i, essenceCost[i].get<int>());
						}			
					}

					if (auto& type = skill["type"]; !skill["type"].is_null()) {
						temp->setSkillType(type);
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
		for (const auto& i : _skillsList) {
			if (i->getNameID() == nameId) {
				_knownSkillsList.push_back(i);
				return true;
			}
		}
		return false;
	}

	bool Player::removeSkill(const std::string& nameId) {
		for (std::size_t i = 0; i < _knownSkillsList.size(); i++)
		{
			if (_knownSkillsList[i]->getNameId() == nameId) {
				_knownSkillsList.erase(_knownSkillsList.begin() + i);
				return true;
			}
		}
		return false;
	}

	Skill* Player::getSkill(const std::string& nameID) {
		for (auto& i : _skillsList) {
			if (i->getNameID() == nameID) {
				return i;
			}
		}
		return nullptr;
	}

	//HP section
	int Player::getHP() {
		return _currentHP;
	}

	void Player::addHP(int HP) {
		_currentHP += HP;
		std::clamp(_currentHP, 0, _maxHP);
	}

	void Player::setHP(int newHP) {
		_currentHP = newHP;
		std::clamp(_currentHP, 0, _maxHP);
	}

	int Player::getMaxHP() {
		return _maxHP;
	}

	void Player::setMaxHP(int newMaxHP) {
		_maxHP = newMaxHP;
	}

	//PP section
	int Player::getPP() {
		return _currentPP;
	}

	void Player::addPP(int PP) {
		_currentPP += PP;
		std::clamp(_currentPP, 0, _maxPP);
	}

	void Player::setPP(int PP) {
		_currentPP = PP;
		std::clamp(_currentPP, 0, _maxPP);
	}

	int Player::getMaxPP() {
		return _maxPP;
	}

	void Player::setMaxPP(int newMaxPP) {
		_maxPP = newMaxPP;
	}
	/*
	void addSkill(std::string& nameID);
	void removeSkill(std::string& nameID);
	Skill getSkill(std::string& nameID);
	*/
}