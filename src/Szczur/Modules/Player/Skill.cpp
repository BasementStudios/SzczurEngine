#include "Skill.hpp"

namespace rat {
	Skill::Skill() {
		icon = new sf::Texture;
	}

	void Skill::initScript(Script& script) {
		auto object = script.newClass<Skill>("Skill", "Player");
		object.set("getName", &Skill::getName);
		object.set("getDescription", &Skill::getDescription);
		object.set("getPPCost", &Skill::getPPcost);
		object.set("getEssenceCost", &Skill::getEssenceCost);
		object.set("getSkillType", &Skill::getSkillType);
		object.set("getIsKnown", &Skill::getIsKnown);
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

	void Skill::setIsKnown(bool _isKnown) {
		isKnown = _isKnown;
	}
	bool Skill::getIsKnown() {
		return isKnown;
	}

	void Skill::setIcon(const std::string& _icon) {
		icon->loadFromFile(_icon);
	}
	sf::Texture* Skill::getIcon() {
		return icon;
	}
}