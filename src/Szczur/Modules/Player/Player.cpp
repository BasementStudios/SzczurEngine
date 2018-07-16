#include "Player.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

namespace rat {
	//skill methods
	void Skill::setNameId(std::string& _nameId) {
		nameId = _nameId;
	}
	std::string& Skill::getNameId() {
		return nameId;
	}

	void Skill::setName(std::string _name) {
		name = _name;
	}
	std::string& Skill::getName() {
		return name;
	}

	void Skill::setDescription(std::string _description) {
		description = _description;
	}
	std::string& Skill::getDescription() {
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

	void Skill::setSkillPath(int path) {
		skillPath = path;
	}
	int Skill::getSkillPath() {
		return skillPath;
	}

	Player::Player() {
		LOG_INFO("Initializing Player module");
		_pathToJson = "Assets/Skills/skills.json";

		initJson();
		initScript();

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

					Skill temp;
					temp.setNameId(itr.key());

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
							temp.setEssenceCost(i, essenceCost[i]);
						}			
					}

					if (auto& skillPath = skill["skillPath"]; !skill["skillPath"].is_null()) {
						temp.setSkillPath(skillPath);
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

	std::string& Player::getPathToJson() {
		return _pathToJson;
	}

	void Player::reloadJson() {
		initJson();
	}

	bool Player::addSkill(std::string& nameId) {
		for (auto& i : _skillsList) {
			if (i.getNameId == nameId) {
				_knownSkillsList.push_back(i);
				return true;
			}
		}
		return false;
	}

	bool Player::removeSkill(std::string& nameId) {
		for (auto it = _knownSkillsList.begin(); it != _knownSkillsList.end(); it++) {
			if (it->getNameId() == nameId) {
				_knownSkillsList.erase(it);
				return true;
			}
		}
		return false;
	}

	Skill& Player::getSkill(std::string& nameId) {
		for (auto& i : _skillsList) {
			if (i.getNameId == nameId) {
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
	void addSkill(std::string& nameId);
	void removeSkill(std::string& nameId);
	Skill getSkill(std::string& nameId);
	*/
}