#include "BattlePawn.hpp"

#include "Battle.hpp"
#include "BattleScene.hpp"
#include "BattleSkill.hpp"

#include <cmath>

#include <Szczur/Modules/World/Scene.hpp>
#include <Szczur/Modules/World/World.hpp>

#include <Szczur/Modules/Script/Script.hpp>

namespace rat {


// ----------------- Constructors -----------------

BattlePawn::BattlePawn(BattleScene* scene) 
	: _scene(scene), 
	_scriptModule(detail::globalPtr<Script>),
	_worldModule(detail::globalPtr<World>),
	_battleModule(detail::globalPtr<Battle>),
	_entity(scene->getScene(), "battles")
{
	// _scriptModule = detail::globalPtr<Script>;
	// _worldModule = detail::globalPtr<World>;

	_armature = static_cast<ArmatureComponent*>(_entity.addComponent<ArmatureComponent>());

	// Radius shape
	_radiusShape.setColor(glm::vec4(0.7f, 0.5f, 0.1f, 0.3f));
	_radiusShape.setRotation(glm::vec3(90.f, 0.f, 0.f));
}

BattlePawn::~BattlePawn()
{

}

// ----------------- Positioning -----------------

void BattlePawn::setPosition(glm::vec2 position) 
{
	_moved = true;
	_position = position;

	// glm::vec3 pos = getScene()->getPosition()+glm::vec3(_position.x, 0.f, _position.y);
	glm::vec3 pos(_position.x, getScene()->getPosition().y, _position.y);
	_entity.setPosition(pos);
	_radiusShape.setPosition(pos);
}

glm::vec2 BattlePawn::getPosition()
{
	return _position;
}

bool BattlePawn::popMovedFlag()
{
	bool flag = _moved;
	_moved = false;
	return flag;
}

void BattlePawn::move(glm::vec2 delta)
{
	_moved = true;
	_position += delta;

	glm::vec3 pos(_position.x, getScene()->getPosition().y, _position.y);
	_entity.setPosition(pos);
	_radiusShape.setPosition(pos);
}

void BattlePawn::move(glm::vec2 normalizedDirection, float distance)
{
	move(normalizedDirection*distance);
}

glm::vec2 BattlePawn::getNormalDirection(glm::vec2 position)
{
	glm::vec2 ret = position - getPosition();
	return glm::normalize(ret);
}

float BattlePawn::getDistanceTo(glm::vec2 position)
{
	return glm::distance(_position, position);
}

// ----------------- Angle -----------------

float BattlePawn::getAngleTo(glm::vec2 position)
{
	glm::vec2 dir = position - getPosition();
	return std::atan2(dir.y, dir.x);
}

// ----------------- Time -----------------

bool BattlePawn::isTimeFull()
{
	return _time >= _maxTime;
}

// ----------------- Damage animation -----------------

void BattlePawn::setDamageAnimation(const std::string& name, float fade, bool wait, float speed)
{
	_isDamageAnimation = true;
	_damageAnimationName = name;
	_damageAnimationFade = fade;
	_damageAnimationWait = wait;
	_damageAnimationSpeed = speed;
}

// ----------------- Collider -----------------

void BattlePawn::setRadius(float radius)
{
	if(radius<0.f) radius = 0.f;
	_radius = radius;
	_radiusShape.setRadius(_radius);
	_radiusShape.setOrigin(glm::vec3(_radius, _radius, 5.f));
}

float BattlePawn::getRadius()
{
	return _radius;
}

void BattlePawn::collision(BattlePawn* target)
{
	if(_onCollision.valid()) {
		_onCollision(this, target);
	}
}

// ----------------- Model -----------------

void BattlePawn::setArmature(const std::string& armDir) 
{
	_entity.getComponentAs<ArmatureComponent>()->setArmature(armDir);
}

ArmatureComponent* BattlePawn::getArmature()
{
	return _armature;
}

const std::string& BattlePawn::getArmaturePath()
{
	return _armature->getName();
}

void BattlePawn::setFlipForPosition(glm::vec2 position)
{
	glm::vec2 dir = position - getPosition();
	float angle = std::atan2(dir.y, dir.x);
	getArmature()->setFlipX(std::fabs(angle)>3.14159f/2.f);
}

void BattlePawn::setOrigin(const glm::vec2& origin)
{
	_entity.setOrigin(glm::vec3(origin.x, origin.y, 0.f));
}

glm::vec2 BattlePawn::getOrigin()
{
	const auto& origin = _entity.getOrigin();
	return glm::vec2(origin.x, origin.y);
}

void BattlePawn::setScale(float scale)
{
	_scale = scale;
	float value = _scale*_scene->getScale();
	_entity.setScale(glm::vec3(value, value, value));
}

float BattlePawn::getScale()
{
	return _scale;
}

void BattlePawn::setHeight(float height)
{
	_height = height;
}

float BattlePawn::getHeight()
{
	return _height;
}

void BattlePawn::renderBars(sf3d::RenderTarget& canvas)
{
	_battleModule->getSpellIndicatorManager().renderHpBarIndicator(_position, _height, _health, _maxHealth);
	_battleModule->getSpellIndicatorManager().renderTimeBarIndicator(_position + glm::vec2(40.f, 0.f), _height-40.f, _time, _maxTime);
	_battleModule->getSpellIndicatorManager().renderStatusBarIndicator(_position + glm::vec2(20.f, 0.f), _height+70.f, _statuses);
}

// ----------------- Skills -----------------

BattleSkill* BattlePawn::newSkill(const std::string& name)
{
	auto& skill = _skills.emplace_back(new BattleSkill());
	skill->setName(name);
	skill->setPawn(this);
	return skill.get();
}

void BattlePawn::renderSkills(sf3d::RenderTarget& canvas)
{
	//@IMP
	if(_onIndicator.valid()) {
		_onIndicator(this, _battleModule->getSpellIndicatorManager());
	}
	// for(auto& skill : _skills) {
	// 	skill->renderSpellIndicator();
	// }
}

void BattlePawn::updateSkills()
{
	for(auto& skill : _skills) {
		skill->update();
	}
}

BattleSkill* BattlePawn::getSkill(int index)
{
	return _skills[index].get();
}

int BattlePawn::getSkillAmount()
{
	return _skills.size();
}

void BattlePawn::setSkills(const std::vector<std::string>& _skillNames)
{
	std::vector<std::unique_ptr<BattleSkill>> newSkills;

	LOG_INFO("Start!")
	for(auto& name : _skillNames) {
		LOG_INFO("Name:", name)
		for(auto& skill : _skills) {
			LOG_INFO("Is null: ", (skill.get()==nullptr));
			if(skill.get() != nullptr) LOG_INFO("Skill:", skill->getName());
			if(skill.get() != nullptr && skill->getName() == name) {
				newSkills.emplace_back(skill.release());
				LOG_INFO("Skill added!");
				break;
			}
		}
	}
	LOG_INFO("Finish!")

	_skills.clear();
	_skills = std::move(newSkills);
	LOG_INFO("Yey!")

	// for(auto itr = _skills.begin(); itr != _skills.end(); ) {
	// 	auto* skill = itr->get();

	// 	bool remove = true; 
	// 	for(auto& name : _skillNames) {
	// 		if(skill->getName() == name) {
	// 			remove = false;
	// 			break;
	// 		}
	// 	}
	// 	if(remove) {
	// 		itr = _skills.erase(itr);
	// 	}
	// 	else {
	// 		++itr;
	// 	}
	// }
}

// ----------------- Statuses -----------------

void BattlePawn::setStatus(int index)
{
	for(auto& status : _statuses) {
		if(status == index) {
			return;
		}
	}
	_statuses.emplace_back(index);
}

bool BattlePawn::checkStatus(int index)
{
	for(auto& status : _statuses) {
		if(status == index) {
			return true;
		}
	}
	return false;
}

void BattlePawn::clearStatuses()
{
	_statuses.clear();
}

// ----------------- Script -----------------

void BattlePawn::loadScript(const std::string& scriptPath)
{
	_scriptPath = scriptPath;
	if(scriptPath != "") {
		_scriptModule->get()["THIS"] = this;
		LOG_INFO("Loading: ", _scriptPath);
		_scriptModule->scriptFile(_scriptPath);
		_scriptModule->get()["THIS"] = sol::nil;
	}
}

const std::string& BattlePawn::getScriptPath()
{
	return _scriptPath;
}

void BattlePawn::initScript(Script& script)
{
	auto object = script.newClass<BattlePawn>("BattlePawn", "Battle");

	// Skills
	object.set("newSkill", &BattlePawn::newSkill);

	// Callbacks
	object.set("onCollision", &BattlePawn::_onCollision);
	object.set("onUpdate", &BattlePawn::_onUpdate);
	object.set("onIndicator", &BattlePawn::_onIndicator); //@IMP

	// Time
	object.set("isTimeFull", &BattlePawn::isTimeFull);

	// Positioning
	object.set("getPosition", &BattlePawn::getPosition);
	object.set("setPosition", &BattlePawn::setPosition);
	object.setOverload("move", 
		sol::resolve<void(glm::vec2)>(&BattlePawn::move),
		sol::resolve<void(glm::vec2, float)>(&BattlePawn::move)
	);

	// Radius
	object.set("getRadius", &BattlePawn::getRadius);
	object.set("setRadius", &BattlePawn::setRadius);

	// Model
	object.set("armature", &BattlePawn::getArmature);
	object.set("setFlipForPosition", &BattlePawn::setFlipForPosition);

	// Math
	object.set("getAngleTo", &BattlePawn::getAngleTo);
	object.set("getNormalDirection", &BattlePawn::getNormalDirection);
	object.set("getDistanceTo", &BattlePawn::getDistanceTo);

	// Health
	object.set("setHealth", &BattlePawn::setHealth);
	object.set("setMaxHealth", &BattlePawn::setMaxHealth);
	object.set("addHealth", &BattlePawn::addHealth);
	object.set("getHealth", &BattlePawn::getHealth);
	object.set("getMaxHealth", &BattlePawn::getMaxHealth);
	object.set("addHealthHit", &BattlePawn::addHealthHit);
	object.set("setHealthHit", &BattlePawn::setHealthHit);

	// Time
	object.set("setTime", &BattlePawn::setTime);
	object.set("setMaxTime", &BattlePawn::setMaxTime);
	object.set("addTime", &BattlePawn::addTime);
	object.set("getTime", &BattlePawn::getTime);
	object.set("getMaxTime", &BattlePawn::getMaxTime);

	// Damage animation
	object.set("setDamageAnimation", &BattlePawn::setDamageAnimation);

	// Statuses
	object.set("setStatus", &BattlePawn::setStatus);
	object.set("checkStatus", &BattlePawn::checkStatus);
	object.set("clearStatuses", &BattlePawn::clearStatuses);

	// Data object
	object.set(sol::meta_function::index, &BattlePawn::_getScriptDataObject);
	object.set(sol::meta_function::new_index, &BattlePawn::_setScriptDataObject);

	object.init();
}

// ----------------- Identification -----------------

void BattlePawn::setNameID(const std::string& name)
{
	_nameID = name;
}

const std::string& BattlePawn::getNameID()
{
	return _nameID;
}

BattleScene* BattlePawn::getScene()
{
	return _scene;
}

// ----------------- Health -----------------

void BattlePawn::setHealth(float value)
{
	if(value<0) value = 0;
	else if(value > _maxHealth) value = _maxHealth;
	_health = value;
}

void BattlePawn::setMaxHealth(float value)
{
	if(value < 1) value = 1;
	_maxHealth = value;
	setHealth(_health);
}

void BattlePawn::addHealth(float value)
{
	setHealth(_health + value);
}

float BattlePawn::getHealth()
{
	return _health;
}

float BattlePawn::getMaxHealth()
{
	return _maxHealth;
}

void BattlePawn::addHealthHit(float value)
{
	setHealth(_health + value);
	if(_isDamageAnimation) {
		_armature->playOnce(_damageAnimationName, _damageAnimationFade, _damageAnimationWait, _damageAnimationSpeed);
	}
}

void BattlePawn::setHealthHit(float value)
{	
	bool flag = value < getHealth();
	setHealth(value);
	if(flag && _isDamageAnimation) {
		_armature->playOnce(_damageAnimationName, _damageAnimationFade, _damageAnimationWait, _damageAnimationSpeed);
	}
}

// ----------------- Time -----------------

void BattlePawn::setTime(float value)
{
	if(value<0) value = 0;
	else if(value >= _maxTime) {

		if(_time < _maxTime && this == detail::globalPtr<Battle>->getPlayer()) {
			detail::globalPtr<Battle>->getCurrentScene()->addPP(1);
		}

		value = _maxTime;
	}
	_time = value;
}

void BattlePawn::setMaxTime(float value)
{
	if(value < 1) value = 1;
	_maxTime = value;
	setTime(_time);
}

void BattlePawn::addTime(float value)
{
	setTime(_time + value);
}

float BattlePawn::getTime()
{
	return _time;
}

float BattlePawn::getMaxTime()
{
	return _maxTime;
}

// ----------------- Main -----------------

void BattlePawn::update(float deltaTime)
{
	clearStatuses();
	_entity.update(_worldModule->getScenes(), deltaTime);
	if(_onUpdate.valid()) {
		_onUpdate(this);
	}
	updateSkills();
	addTime(deltaTime*10.f);
}

void BattlePawn::render(sf3d::RenderTarget& target)
{
	target.draw(_entity);
}

void BattlePawn::bottomRender(sf3d::RenderTarget& target)
{
	target.draw(_radiusShape);
}

// ----------------- Data object -----------------

void BattlePawn::_setScriptDataObject(std::string key, sol::stack_object value) {
	auto obj = _scriptData.find(key);

	if (obj == _scriptData.cend()) {
		_scriptData.insert(obj, { std::move(key), std::move(value) });
	}
	else {
		obj->second = sol::object(std::move(value));
	}
}

sol::object BattlePawn::_getScriptDataObject(const std::string& key) {
	auto obj = _scriptData.find(key);
	return (obj == _scriptData.cend() ? sol::lua_nil : obj->second);
}

} // namespace rat