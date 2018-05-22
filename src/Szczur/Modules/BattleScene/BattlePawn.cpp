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

}

void BattlePawn::render(BattlePawn::RenderTarget& canvas) {
	// Pose
	if(texturePose.good()) {
		sf::Sprite sprite(texturePose.getSprite(frame));
		sprite.setScale(1-2*flip, 1);
		sprite.setOrigin(sprite.getTextureRect().width/2.f, sprite.getTextureRect().height);
		sprite.setPosition(pos);	
		canvas.draw(sprite);	
		renderHpBar(canvas, 30.f);
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

void BattlePawn::renderHpBar(sf::RenderTarget& canvas, float oy) {
	sf::CircleShape shape(13.f);
	shape.setOrigin(13.f, 13.f);
	shape.setFillColor({0,0,0,0});
	shape.setOutlineColor({107,29,52,200});
	shape.setOutlineThickness(-3);

	shape.setPosition(pos.x-28, pos.y+oy);
	int orbs = std::ceil(maxHealth/30.f);

	for(int i=0; i<orbs; ++i) {
		shape.move(28,0);
		canvas.draw(shape);
	}

	shape.setOutlineColor({0,0,0,0});
	shape.setFillColor({224,31,113,200});
	shape.setPosition(pos.x, pos.y+oy);
	shape.setRadius(7.f);
	shape.setOrigin(7.f,7.f);
	float temp = health;
	while(true) {
		if(temp>=30) {
			canvas.draw(shape);
		}
		else break;
		temp-=30;
		shape.move(28,0);
	}
	if(temp>0) {
		float radius = 7.f*(temp/30.f);
		shape.setRadius(radius);
		shape.setOrigin(radius, radius);
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
	object.set("getPosition", &BattlePawn::getPosition);
	object.set("moveInDirection", &BattlePawn::moveInDirection);
	object.set("setFrame", &BattlePawn::setFrame);
	object.set("setFlip", &BattlePawn::setFlip);
	object.set("getDirection", &BattlePawn::getDirection);
	object.set("addUsable", &BattlePawn::addUsable);
	object.set("removeUsable", &BattlePawn::removeUsable);
	object.set("clearUsable", &BattlePawn::removeUsable);

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