#include "BattleSkill.hpp"

#include <Szczur/Modules/Input/Input.hpp>

namespace rat {

// ========== Constructors ==========

BattleSkill::BattleSkill(BattlePawn* pawn, const std::string& name) 
	: pawn(pawn), name(name), input(*detail::globalPtr<Input>) {
}

// ========== Main ==========

void BattleSkill::init() {
	if(onInit.valid()) {
		onInit(this*);
	}
}

void BattleSkill::update(float deltaTime) {
	if(onUpdate.valid()) {
		onUpdate(this*, deltaTime);
	}
}

// ========== Getters ==========

void BattleSkill::getPawn() {
	return pawn;
}


bool BattleSkill::isFinished() {
	return killed;
}

const std::string& BattleSkill::getName() const {
	return name;
}

// ========== Manipulations ==========

void BattleSkill::kill() {
	killed = true;
}

void BattleSkill::setType(size_t type) {
	this->type = type;
}

// ========== Controller ==========

void BattleSkill::renderCircle(float radius) {
	sf::CircleShape shape(radius);
	shape.setFillColor({0,0,0,0});
	shape.setOutlineColor({255,0,150,150});
	shape.setOutlineThickness(-2);
	shape.setOrigin(radius, radius);
	shape.setPosition(sf::Vector2f(input.getManager().getMousePosition()));
}

// ========== Scripts ==========

void BattleSkill::initScript(Script& script) {
	auto object = script.newClass<BattleSkill>("BattleSkill", "BattleScene");

	object.set("setType", [](BattleSkill& skill, sol::variadic_args args){
		size_t type = 0;
		for(auto& obj : args) {
			if(obj.get<std::string>() == "AUTO") {
				type |= SELECT_AUTO;
			}
			else if(obj.get<std::string>() == "CURSOR") {
				type |= SELECT_CURSOR;
			}
			else if(obj.get<std::string>() == "ENEMY") {
				type |= SELECT_ENEMY;
			}
			else if(obj.get<std::string>() == "FRIEND") {
				type |= SELECT_FRIEND;
			}
			else if(obj.get<std::string>() == "OBJECT") {
				type |= SELECT_OBJECT;
			}
			else if(obj.get<std::string>() == "ANY") {
				type |= SELECT_ANY;
			}
		}
		skill.setType(type);
	});
	object.set("onUpdate", &BattleSkill::onUpdate);
	object.set("onInit", &BattleSkill::onInit);
	object.set("onActive", &BattleSkill::onActive);
	object.set("getPawn", &BattleSkill::getPawn);
	object.set("renderCircle", &BattleSkill::renderCircle);

	object.init();
}

// ========== BattleSkillMove ==========

// void BattleSkillMove::init() {
// 	float d = posEnd - posBegin;
// 	angle = std::atan2(d.y, d.x);
// 	distance = std::sqrt(d.x*d.x + d.y*d.y);
// 	clock.restart();
// }

// void BattleSkillMove::update(float deltaTime) {
// 	getPawn().move(std::cos(angle)*deltaTime*10.f, std::sin(angle)*deltaTime*10.f);
// 	if(clock.getElapsedTime().asSeconds()>1.5f) {
// 		kill();
// 		if(onSignal.valid()) {
// 			onSignal();
// 		}
// 	}
// }

}