#include "BattlePawn.hpp"

#include <cmath>

#include <json.hpp>
using Json = nlohmann::json;

#include <Szczur/Modules/Input/Input.hpp>
#include <Szczur/Modules/Script/Script.hpp>

#include "BattleScene.hpp"
#include "BattleSkill.hpp"

namespace rat {

// ========== Constructors ========== 

BattlePawn::BattlePawn(BattleScene& battleScene) 
	: battleScene(battleScene), input(*detail::globalPtr<Input>) {
}
 
// ========== Main ========== 

void BattlePawn::update(float deltaTime) {
	addCooldown(10.f*deltaTime);
}

void BattlePawn::render(BattlePawn::RenderTarget& canvas) {
	// Pose
	if(texturePose.good()) {
		sf::Sprite sprite(texturePose.getSprite(frame));
		sprite.setScale(1-2*flip, 1);
		sprite.setOrigin(sprite.getTextureRect().width/2.f, sprite.getTextureRect().height);
		sprite.setPosition(pos);	

		// OPENGL

		canvas.draw(sprite);	
		
		// --OPENGL

		renderHpBar(canvas, 30.f);
		renderCooldownBar(canvas, 70.f);
	}
}

// ========== Getters ========== 

BattleScene* BattlePawn::getScene() {
	return &battleScene;
}

const sf::Vector2f& BattlePawn::getPosition() const {
	return pos;
}

float BattlePawn::getColliderRadius() const {
	return colliderRadius;
}

bool BattlePawn::hitTest(const sf::Vector2f& point) const {
	sf::Vector2f d = pos - point;
	return std::sqrt(d.x*d.x+d.y*d.y)<colliderRadius;
}

bool BattlePawn::getDirection(float angle) const {
	return std::fabs(angle)>3.14159f/2.f;
}

const std::vector<std::unique_ptr<BattleSkill>>& BattlePawn::getSkills() const {
	return skills;
}

sf::Sprite BattlePawn::getIconSprite(int iconFrame) const {
	return textureSkillIcons.getSprite(iconFrame);
}

const std::vector<std::pair<BattleSkill*, int>>& BattlePawn::getUsableSkills() const {
	return usableSkills;
}

// ========== Manipulations ========== 

void BattlePawn::setPosition(const sf::Vector2f& position) {
	pos = position;
	battleScene.fixPosition(*this);
}
void BattlePawn::setPositionX(float x) {
	pos.x = x;
	battleScene.fixPosition(*this);
}
void BattlePawn::setPositionY(float y) {
	pos.y = y;
	battleScene.fixPosition(*this);
}

void BattlePawn::setRawPosition(const sf::Vector2f& position) {
	pos = position;
}
void BattlePawn::setRawPositionX(float x) {
	pos.x = x;
}
void BattlePawn::setRawPositionY(float y) {
	pos.y = y;
}

void BattlePawn::move(const sf::Vector2f& vector) {
	pos += vector;
	battleScene.fixPosition(*this);
}

void BattlePawn::moveRaw(const sf::Vector2f& vector) {
	pos += vector;
}

float BattlePawn::getAngleTo(const sf::Vector2f& pos) {
	sf::Vector2f delta = pos - this->pos;
	return std::atan2(delta.y, delta.x);
}

float BattlePawn::getAngleToPointer() {
	sf::Vector2f delta = sf::Vector2f(input.getManager().getMousePosition()) - this->pos;
	return std::atan2(delta.y, delta.x);
}
 
void BattlePawn::moveInDirection(float angle, float distance) {
	pos.x += std::cos(angle)*distance;
	pos.y += std::sin(angle)*distance;
	battleScene.fixPosition(*this);
}

void BattlePawn::loadPawn(const std::string& dirPath) {
	Json config;
	std::ifstream(dirPath+"/config.json") >> config;

	if(auto& pose = config["textures"]["pose"]; !pose.is_null()) {
		texturePose.load(dirPath+"/"+pose[0].get<std::string>(), pose[1], pose[2], pose[3], pose[4], pose[5]);
	}
	if(auto& icons = config["textures"]["skill_icons"]; !icons.is_null()) {
		textureSkillIcons.load(dirPath+"/"+icons[0].get<std::string>(), icons[1], icons[2], icons[3], icons[4], icons[5]);
	}

	if(!config["scripts"].is_null()) {
		for(auto& obj : config["scripts"]) {
			runScript(dirPath+"/"+obj.get<std::string>());
		}
	}
}
 
void BattlePawn::setCollisionStatus(bool flag) {
	collision = flag;
}

bool BattlePawn::isCollision() {
	return collision;
}

float BattlePawn::getDistanceToPointer() {
	sf::Vector2f d = pos - sf::Vector2f(input.getManager().getMousePosition());
	return std::sqrt(d.x*d.x+d.y*d.y);
}

float BattlePawn::getDistanceTo(const sf::Vector2f& pos) {
	sf::Vector2f d = this->pos - pos;
	return std::sqrt(d.x*d.x+d.y*d.y); 
}

// ========== Battle ========== 

// Health

void BattlePawn::addHp(float value) {
	setHp(health+value);
}

	 
void BattlePawn::setHp(float value) {
	if(value<0) value = 0;
	else if(value>maxHealth) value = maxHealth;
	health = value;
}

	 
void BattlePawn::setMaxHp(float value) {	
	if(value<1) value = 1;
	maxHealth = value;
	setHp(maxHealth);
}

	 
float BattlePawn::getHp() const {
	return health;
}

float BattlePawn::getMaxHp() const {
	return maxHealth;
}

// Cooldown

void BattlePawn::addCooldown(float value) {
	setCooldown(cooldown+value);
}

	 
void BattlePawn::setCooldown(float value) {
	if(value<0) value = 0;
	else if(value>maxCooldown) value = maxCooldown;
	cooldown = value;
}

	 
void BattlePawn::setMaxCooldown(float value) {	
	if(value<1) value = 1;
	maxCooldown = value;
	setCooldown(maxCooldown);
}

	 
float BattlePawn::getCooldown() const {
	return cooldown;
}

	 
float BattlePawn::getMaxCooldown() const {
	return maxCooldown;
}

// ========== Skills ========== 

BattleSkill* BattlePawn::newSkill(const std::string& skillName) {
	return skills.emplace_back(new BattleSkill(this, skillName)).get();
}

BattleSkill* BattlePawn::getSkill(const std::string& skillName) {
	auto result = std::find_if(skills.begin(), skills.end(), 
		[&](const auto& obj){ return obj->getName()==skillName; }
	);
	if(result != skills.end()) {
		return result->get();
	}
	else {
		return nullptr;
	}
}

BattleSkill* BattlePawn::useSkill(BattleSkill* skill) {
	battleScene.useSkill(skill);
}

BattleSkill* BattlePawn::useSkill(const std::string& skillName) {
	return useSkill(getSkill(skillName));
}

void BattlePawn::addUsable(BattleSkill* skill, int icon) {
	if(skill == nullptr) return;
	for(auto& obj : usableSkills) {
		if(obj.first == skill) return;
	}
	usableSkills.emplace_back(skill, icon);
}

void BattlePawn::removeUsable(BattleSkill* skill) {
	if(skill == nullptr) return;
	for(auto itr  = usableSkills.begin(); itr!=usableSkills.end(); ++itr) {
		if(itr->first == skill) {
			usableSkills.erase(itr);
			return;
		}
	}
}

void BattlePawn::clearUsable() {
	usableSkills.clear();
}

// ========== Controller ========== 

void BattlePawn::renderController(BattlePawn::RenderTarget& canvas, bool selected) const {
	sf::CircleShape collider(colliderRadius);
	collider.setOrigin(colliderRadius, colliderRadius);
	collider.setPosition(pos);
	if(!selected) {
		collider.setFillColor({150,0,255,100});
		collider.setOutlineColor({150,0,255,200});
	}
	else {
		collider.setFillColor({0,150,255,100});
		collider.setOutlineColor({0,150,255,200});
	}
	collider.setOutlineThickness(-2);
	canvas.draw(collider);
}

// ========== Visual ========== 

void BattlePawn::setFrame(int frame) {
	this->frame = frame;
}

void BattlePawn::setFlip(bool flag) {
	flip = flag;
}

void BattlePawn::renderCooldownBar(sf::RenderTarget& canvas, float oy) {
	sf::CircleShape shape(8.f);
	shape.setOrigin(8.f, 8.f);
	shape.setFillColor({0,0,0,0});
	shape.setOutlineColor({130,130,90,200});
	shape.setOutlineThickness(-2);

	shape.setPosition(pos.x-(8.f*2.f+2.f)+8.f, pos.y+oy);
	int orbs = std::ceil(maxCooldown/20.f);

	for(int i=0; i<orbs; ++i) {
		shape.move((8.f*2.f+2.f),0);
		canvas.draw(shape);
	}

	shape.setOutlineColor({50,50,30,180});
	shape.setOutlineThickness(1);
	shape.setFillColor({220,220,170,240});
	shape.setPosition(pos.x+8.f, pos.y+oy);
	shape.setRadius(6.f);
	shape.setOrigin(6.f,6.f);
	float temp = cooldown;
	while(temp>=20) {
		canvas.draw(shape);
		temp-=20;
		shape.move((8.f*2.f+2.f),0);
	}
	if(temp>0) {
		float radius = (6.f)*(temp/20.f);
		shape.setFillColor({200,200,150,200});
		shape.setRadius(radius);
		shape.setOrigin(radius, radius);
		canvas.draw(shape);
	}
}


void BattlePawn::renderHpBar(sf::RenderTarget& canvas, float oy) {
	sf::RectangleShape shape;

	float maxWidth = 100.f;
	float width = maxWidth*health/maxHealth;
	float segmentWidth = maxWidth*30.f/maxHealth;

	int amountLines = std::floor(maxHealth/30.f);

	shape.setSize({maxWidth, 10.f});
	shape.setFillColor({224,31,113,80});
	shape.setOutlineThickness(2); 
	shape.setOutlineColor({107,29,52,200}); 
	shape.setOrigin(0.f, 0.f);
	shape.setPosition(pos.x, pos.y+oy);
	canvas.draw(shape);

	shape.setOutlineThickness(0);
	shape.setFillColor({224,31,113,200});
	shape.setSize({width, 10.f});
	canvas.draw(shape);

	shape.setSize({1.f,5.f});
	shape.setOrigin({0.f,0.f});
	shape.setOutlineThickness(1);
	shape.setFillColor({224,31,113,200});
	for(int i=0; i<amountLines; ++i) {
		shape.move(segmentWidth, 0);
		canvas.draw(shape);
	}

}

// ========== Scripts ========== 

void BattlePawn::initScript(Script& script) {
	auto object = script.newClass<BattlePawn>("BattlePawn", "BattleScene");

	object.set("newSkill", &BattlePawn::newSkill);
	object.setOverload("useSkill", 
		sol::resolve<BattleSkill*(BattleSkill*)>(&BattlePawn::useSkill), 
		sol::resolve<BattleSkill*(const std::string&)>(&BattlePawn::useSkill)
	);
	object.set("getSkill", &BattlePawn::getSkill);
	object.set("getAngleTo", &BattlePawn::getAngleTo);
	object.set("getAngleToPointer", &BattlePawn::getAngleToPointer);
	object.set("getPosition", [](BattlePawn& o){return o.getPosition();});
	object.set("getDistanceToPointer", &BattlePawn::getDistanceToPointer);
	object.set("getDistanceTo", &BattlePawn::getDistanceTo);
	object.set("move", &BattlePawn::moveInDirection);
	object.set("setFrame", &BattlePawn::setFrame);
	object.set("setFlip", &BattlePawn::setFlip);
	object.set("getDirection", &BattlePawn::getDirection);
	object.set("addUsable", &BattlePawn::addUsable);
	object.set("removeUsable", &BattlePawn::removeUsable);
	object.set("clearUsable", &BattlePawn::removeUsable);
	object.set("setCollisionStatus", &BattlePawn::setCollisionStatus);
	object.set("isCollision", &BattlePawn::isCollision);
	
	// Health
	object.set("addHp", &BattlePawn::addHp);
	object.set("setHp", &BattlePawn::setHp);
	object.set("setMaxHp", &BattlePawn::setMaxHp);
	object.set("getHp", &BattlePawn::getHp);
	object.set("getMaxHp", &BattlePawn::getMaxHp);

	object.init();
}

void BattlePawn::runScript(const std::string& filePath) {
	auto& script = *detail::globalPtr<Script>;
	try {
		script.get()["THIS"] = this;
		script.scriptFile(filePath);
		script.get()["THIS"] = sol::nil;
	}
	catch(std::exception& exc) {
		LOG_INFO(exc.what());
	}
}

}