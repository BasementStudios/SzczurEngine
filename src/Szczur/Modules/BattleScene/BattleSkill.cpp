#include "BattleSkill.hpp"

#include <Szczur/Modules/Input/Input.hpp>

#include "BattlePawn.hpp"
#include "BattleTrigger.hpp"
#include "BattleScene.hpp"

namespace rat {

// ========== Constructors ==========

BattleSkill::BattleSkill(BattlePawn* pawn, const std::string& name) 
	: pawn(pawn), name(name), input(detail::globalPtr<Input>->getManager()) {
}


BattleSkill::BattleSkill(const BattleSkill& obj) 
	: pawn(obj.pawn), name(obj.name), input(detail::globalPtr<Input>->getManager()) {

	onInit = obj.onInit;
	onUpdate = obj.onUpdate;
	onProvide = obj.onProvide;
	data = obj.data;
	selectType = obj.selectType;	
	killed = obj.killed;
}

// ========== Main ==========

void BattleSkill::init() {
	if(onInit.valid()) {
		killed = false;
		onInit(this);
	}
	inited = true;
}

void BattleSkill::update(float deltaTime) {
	if(onUpdate.valid()) {
		onUpdate(this, deltaTime);
	} 
	else {
		killed = true;
	}
}

// ========== Getters ==========

BattlePawn* BattleSkill::getPawn() {
	return pawn;
}

bool BattleSkill::isFinished() {
	return killed;
}

const std::string& BattleSkill::getName() const {
	return name;
}

size_t BattleSkill::getType() const {
	return selectType;
}

bool BattleSkill::isInited() const {
	return inited;
}

// ========== Manipulations ==========

void BattleSkill::kill() {
	killed = true;
}

void BattleSkill::setType(size_t type) {
	selectType = type;
}

BattleTrigger* BattleSkill::makeTrigger() {
	BattleTrigger* ret = new BattleTrigger(pawn);
	pawn->getScene()->addTrigger(ret);
	return ret;
}

void BattleSkill::setPawn(BattlePawn* pawn) {
	this->pawn = pawn;
}

// ========== Controller ==========

void BattleSkill::renderCircle(sf::RenderTarget& canvas, float radius) {
	sf::CircleShape shape(radius);
	shape.setFillColor({0,0,0,0});
	shape.setOutlineColor({255,0,150,150});
	shape.setOutlineThickness(-2);
	shape.setOrigin(radius, radius);
	shape.setPosition(sf::Vector2f(input.getMousePosition()));
	canvas.draw(shape);
}

void BattleSkill::renderController(sf::RenderTarget& canvas) {
	if(onProvide.valid()) {
		onProvide(this, canvas);
	}
}

void BattleSkill::updateController(BattlePawn* selectedPawn) {
	if(selectType & SELECT_SPACE && input.isPressed(Mouse::Left)) {
		getPawn()->useSkill(this);
	}
}

// ========== Scripts ==========

void BattleSkill::initScript(Script& script) {
	auto object = script.newClass<BattleSkill>("BattleSkill", "BattleScene");

	object.set("setType", [](BattleSkill& skill, sol::variadic_args args){
		size_t type = 0;
		for(const std::string& obj : args) {

			if(obj == "SPACE") {
				type |= SELECT_SPACE;
			}
			else if(obj == "ENEMY") {
				type |= SELECT_ENEMY;
			}
			else if(obj == "FRIEND") {
				type |= SELECT_FRIEND;
			}
			else if(obj == "OBJECT") {
				type |= SELECT_OBJECT;
			}
			else if(obj == "SELF") {
				type |= SELECT_SELF;
			}
		}
		skill.setType(type);
	});
	object.set("getName", &BattleSkill::getName);
	object.set("onUpdate", &BattleSkill::onUpdate);
	object.set("onInit", &BattleSkill::onInit);
	object.set("onProvide", &BattleSkill::onProvide);
	object.set("getPawn", &BattleSkill::getPawn);
	object.set("renderCircle", &BattleSkill::renderCircle);
	object.set("kill", &BattleSkill::kill);
	object.set("makeTrigger", &BattleSkill::makeTrigger);
	object.set(sol::meta_function::index, &BattleSkill::getVariable);
	object.set(sol::meta_function::new_index, &BattleSkill::setVariable);

	object.init();
}

void BattleSkill::setVariable(std::string key, sol::stack_object object) {
	auto itr = data.find(key);
	if(itr == data.end()) {
		data.insert(itr, {std::move(key), std::move(object)});
	}
	else {
		itr->second = sol::object(std::move(object));
	}
}

sol::object BattleSkill::getVariable(const std::string& key) {
	auto itr = data.find(key);
	if(itr == data.end()) {
		return sol::lua_nil;
	}
	return itr->second;
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