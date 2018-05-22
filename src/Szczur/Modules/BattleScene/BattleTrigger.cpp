#include "BattleTrigger.hpp"

#include "BattlePawn.hpp"
#include "BattleScene.hpp"

namespace rat {

// ========== Main ========== 

BattleTrigger::BattleTrigger(BattlePawn* caster) 
	: caster(caster), scene(caster->getScene()) {
	shape.setRadius(radius);
	shape.setOrigin(radius, radius);
	shape.setFillColor({40,120,180,180});
	shape.setOutlineColor({60,150,220,180});
	shape.setOutlineThickness(-4);
	shape.setPosition(position);
}

// ========== Getters ========== 

bool BattleTrigger::isFinished() const {
	return killed;
}

BattlePawn* BattleTrigger::getCaster() {
	return caster;
}

// ========== Main ========== 

void BattleTrigger::render(sf::RenderTarget& canvas) {

	if(targets.empty()) {
		shape.setFillColor({40,120,180});
		shape.setOutlineColor({60,150,220});
	}
	else {		
		shape.setFillColor({180,120,40});
		shape.setOutlineColor({220,150,60});
	}

	sf::Color color = shape.getOutlineColor();
	color.a = 180.f*(1-clock.getElapsedTime().asSeconds()/0.5f);
	shape.setOutlineColor(color);
	if(float elapsed = clock.getElapsedTime().asSeconds()<=0.25f) {
		color = shape.getFillColor();
		color.a = 180.f*(1-clock.getElapsedTime().asSeconds()/0.25f);
		shape.setFillColor(color);
	}
	else {
		shape.setFillColor({0,0,0,0});
	}
	canvas.draw(shape);
}


void BattleTrigger::update(float deltaTime) {
	if(clock.getElapsedTime().asSeconds()>0.5f) {
		killed = true;
	}
}


void BattleTrigger::init() {
	clock.restart();
	updateTargets();
	if(onInit.valid() && !targets.empty()) {
		onInit(this);
	}
}

// ========== Manipulations ========== 


void BattleTrigger::setRadius(float radius) {
	this->radius = radius;
	shape.setRadius(radius);
	shape.setOrigin(radius, radius);
}


void BattleTrigger::setPosition(float x, float y) {
	position.x = x;
	position.y = y;
	shape.setPosition(position);
}


void BattleTrigger::setPosition(const sf::Vector2f& pos) {
	position = pos;
	shape.setPosition(position);
}

// ========== Targeting ========== 

void BattleTrigger::updateTargets() {
	if(nearestOnlyType) {
		targetNearest();
	}
}

void BattleTrigger::targetNearest() {

	const auto& pawns = scene->getPawns();

	float min = 100000000;
	BattlePawn* target = nullptr;

	float dis;
	for(auto& obj : pawns) {
		dis = getSemiDistanceTo(obj->getPosition());
		if(min>dis && obj.get() != caster) {
			min = dis;
			target = obj.get();
		}
	}

	if(target != nullptr) {
		if(sqrt(min)<=radius) {
			targets.emplace_back(target);
		}
	}
}

void BattleTrigger::nearestOnly() {
	nearestOnlyType = true;
}

BattlePawn* BattleTrigger::getTarget() {
	if(targets.empty()) return nullptr;
	return targets.front();
}

// ========== Math ========== 

float BattleTrigger::getSemiDistanceTo(const sf::Vector2f& pos) {
	sf::Vector2f d = position-pos;
	return d.x*d.x+d.y*d.y;
}


float BattleTrigger::getDistanceTo(const sf::Vector2f& pos) {
	sf::Vector2f d = position-pos;
	return std::sqrt(d.x*d.x+d.y*d.y);
}

// ========== Scripts ========== 


void BattleTrigger::setVariable(std::string key, sol::stack_object object) {
	auto itr = data.find(key);
	if(itr == data.end()) {
		data.insert(itr, {std::move(key), std::move(object)});
	}
	else {
		itr->second = sol::object(std::move(object));
	}
}

sol::object BattleTrigger::getVariable(const std::string& key) {
	auto itr = data.find(key);
	if(itr == data.end()) {
		return sol::lua_nil;
	}
	return itr->second;
}

BattleTrigger::initScript(Script& script) {
	auto object = script.newClass<BattleTrigger>("BattleTrigger", "BattleScene");
	object.set("getCaster", &BattleTrigger::getCaster);
	object.set("setRadius", &BattleTrigger::setRadius);
	object.set("nearestOnly", &BattleTrigger::nearestOnly);
	object.set("getTarget", &BattleTrigger::getTarget);
	object.setOverload("setPosition", 
		sol::resolve<void(float,float)>(&BattleTrigger::setPosition),
		sol::resolve<void(const sf::Vector2f&)>(&BattleTrigger::setPosition)
	);
	object.set("onInit", &BattleTrigger::onInit);
	object.set(sol::meta_function::index, &BattleTrigger::getVariable);
	object.set(sol::meta_function::new_index, &BattleTrigger::setVariable);
	object.init();
}

}