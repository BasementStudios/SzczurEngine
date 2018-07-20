#include "BattleScene.hpp"

#include "Battle.hpp"
#include "BattlePawn.hpp"
#include "BattleSkill.hpp"
#include "BattleEffect.hpp"
#include "BattleSprite.hpp"
#include "BattleTrigger.hpp"
#include "BattleAnimationSprite.hpp"

#include <Szczur/Modules/Script/Script.hpp>
#include <Szczur/Modules/World/Scene.hpp>
#include <Szczur/Modules/World/World.hpp>
#include <Szczur/Modules/Input/Input.hpp>
#include <Szczur/Modules/Player/Player.hpp>

namespace rat {


// ----------------- Constructors -----------------

BattleScene::BattleScene(const glm::vec3& position, const glm::vec2& size, float scale) 
{
	_battleModule = detail::globalPtr<Battle>;
	//_windowModule = detail::globalPtr<Window>;
	_inputModule = detail::globalPtr<Input>;

	_scene = detail::globalPtr<World>->getScenes().getCurrentScene();

	_areaShape.setRotation({ -90.f, 0.f, 0.f });
   	_areaShape.setColor({ 0.3f, 0.60f, 0.80f, 0.3f }); 

   	setParameters(position, size, scale);
}

BattleScene::~BattleScene()
{
	;
}

// ----------------- Main parameters -----------------

void BattleScene::setParameters(const glm::vec3& position, const glm::vec2& size, float scale)
{
	// Set size
	_size = size;

	// Set position (convert center position to left-top corner position)
	_position = position - glm::vec3(_size.x/2.f, 0.f, _size.y/2.f);
	
	// Update field of battle scene
   	_areaShape.setSize({_size.x, _size.y}); 
   	_areaShape.setPosition(_position); 
   	_areaShape.setOrigin({ 0.f, 0.f, 0.f });

   	// Set global scale for all pawns
	_scale = scale;

	// Update pawns (if they are on scene)
	for(auto& pawn : _pawns) {
		// pawn->setPosition(pawn->getPosition());
		pawn->setScale(pawn->getScale());
	}
}

glm::vec3 BattleScene::getPosition()
{
	return _position;
}

glm::vec2 BattleScene::getSize()
{
	return _size;
}

float BattleScene::getScale()
{
	return _scale;
}

// ----------------- Callbacks -----------------

void BattleScene::finish()
{
	if(_onFinish.valid()) {
		_onFinish(this, bool(getPlayer()->getHealth()>0));
		_onFinish = sol::function();
	}
}

// ----------------- Parent scene -----------------

Scene* BattleScene::getScene()
{
	return _scene;
}

// ----------------- Pawns -----------------

BattlePawn* BattleScene::addPawn(const std::string& nameID, const glm::vec2& position)
{
	auto newPawn = _battleModule->getPawnManager().createPawn(nameID, this);
	
	if(!newPawn) 
		return nullptr;

	auto& pawn = _pawns.emplace_back(std::move(newPawn));

	glm::vec3 pos = getPosition();
	pawn->setPosition(position + glm::vec2(pos.x, pos.z));
	return pawn.get();
}

BattlePawn* BattleScene::addPlayer(const std::string& nameID, const glm::vec2& position)
{
	// Create and add pawn
	BattlePawn* pawn = addPawn(nameID, position);

	// Set player (controller)
	_player = pawn;

	return pawn;
}

void BattleScene::reloadAllPawns() 
{	
	std::vector<std::unique_ptr<BattlePawn>> oldPawns(std::move(_pawns)); 

	for(auto& oldPawn : oldPawns) {
		auto* newPawn = addPawn(oldPawn->getNameID(), oldPawn->getPosition());
		if(newPawn != nullptr) {
			newPawn->setPosition(oldPawn->getPosition());
		}
	}
}

void BattleScene::updateRemovingPawns()
{
	for(auto itr = _pawns.begin(); itr != _pawns.end(); ) {
		if(itr->get() != _player && (*itr)->getHealth() == 0) {
			removePawnFromEffects(itr->get());
			itr = _pawns.erase(itr);
		}
		else {
			++itr;
		}
	}
}

std::vector<std::unique_ptr<BattlePawn>>& BattleScene::getPawns()
{
	return _pawns;
}

BattlePawn* BattleScene::getPlayer()
{
	return _player;
}

// ----------------- Effects -----------------

BattleEffect* BattleScene::newEffect()
{
	return _effects.emplace_back(new BattleEffect()).get();
}

void BattleScene::removePawnFromEffects(BattlePawn* pawn) {
	for(auto& effect : _effects) {
		effect->clearPawnFromData(pawn);
	}
}

// ----------------- Triggers -----------------

BattleTrigger* BattleScene::newTrigger()
{
	return _triggers.emplace_back(new BattleTrigger(this)).get();	
}

// ----------------- Sprites -----------------

BattleSprite* BattleScene::newSprite(const std::string& textureName)
{
	auto* newSprite = _sprites.emplace_back(new BattleSprite(this)).get();
	newSprite->setTexture(textureName);
	return newSprite;
}

BattleAnimationSprite* BattleScene::newAnimationSprite(const std::string& textureName)
{	
	auto* newSprite = static_cast<BattleAnimationSprite*>(_sprites.emplace_back(new BattleAnimationSprite(this)).get());
	newSprite->setTexture(textureName);
	return newSprite;
}

void BattleScene::removeSprite(BattleSprite* sprite)
{
	for(auto itr = _sprites.begin(); itr != _sprites.end(); ) {
		if(itr->get() == sprite) {
			itr = _sprites.erase(itr);
			return;
		}
		else {
			++itr;
		}
	}
}

// ----------------- State -----------------

bool BattleScene::isActive()
{
	return _activated;
}

void BattleScene::activate()
{
	_activated = true;
	_selectedSkill = nullptr;
}

// ----------------- Cursor -----------------

glm::vec2 BattleScene::getCursorPosition()
{
	glm::vec3 pos = _battleModule->getCursorPosition(getPosition().y);
	return glm::vec2(pos.x, pos.z) - glm::vec2(_position.x, _position.z);
}

// ----------------- Collision -----------------

void BattleScene::updateCollision()
{
	BattlePawn* last = nullptr;
	for(auto& pawn : _pawns) {
		
		// Collision with other pawns
		last = nullptr;
		if(pawn->popMovedFlag()) {

			for(auto& other : _pawns) {
				
				if(pawn.get() == other.get()) 
					continue;

				float rr = pawn->getRadius() + other->getRadius();

				glm::vec2 dir = pawn->getPosition() - other->getPosition();
				float dis = glm::length(dir);
				
				if(dis < rr) {
					pawn->move(glm::normalize(dir)*(rr-dis+2.f));
					last = other.get();
				}
			}
		}

		if(last != nullptr) {
			pawn->collision(last);
		}

		// Border of battle field
		// > X
		bool flag = true;
		glm::vec2 pos = pawn->getPosition();
		if(pos.x < _position.x + pawn->getRadius()) {
			flag = true;
			pawn->setPosition({_position.x + pawn->getRadius(), pos.y});
		}
		else if(pos.x > _position.x + _size.x - pawn->getRadius()) {
			flag = true;
			pawn->setPosition({_position.x + _size.x  - pawn->getRadius(), pos.y});
		}
		// > Y
		pos = pawn->getPosition();
		if(pos.y < _position.z + pawn->getRadius()) {
			flag = true;
			pawn->setPosition({pos.x, _position.z + pawn->getRadius()});
		}
		else if(pos.y > _position.z + _size.y - pawn->getRadius()) {
			flag = true;
			pawn->setPosition({pos.x, _position.z + _size.y  - pawn->getRadius()});
		}
		if(flag) {
			pawn->collision(nullptr);
		}
	}
}

// ----------------- GUI -----------------

void BattleScene::updateGuiParameters()
{

	Player* gui = detail::globalPtr<Player>;
	gui->setMaxHP(getPlayer()->getMaxHealth());
	gui->setHP(getPlayer()->getHealth());
	
	gui->setMaxTime(getPlayer()->getMaxTime());
	gui->setCurrentTime(getPlayer()->getTime());

	gui->setPPSlotsAmount(getMaxPP(), getBrokenPP());
	gui->setMaxPP(getMaxPP());
	gui->setPP(getPP());
	gui->setHighlightedPPAmount(0);

	gui->clearSkillSlots();

	for(int i = 0; i<getPlayer()->getSkillAmount(); ++i)
	{
		auto* skill = getPlayer()->getSkill(i);

		// auto* guiSkill = gui->getSkill(skill->getName());
	// 	guiSkill->setPPCost(skill->getCostPP());
		gui->setPPCost(skill->getName(), skill->getCostPP());
		gui->addSkillToSlot(skill->getName());

		if(skill == _selectedSkill) {
			gui->chooseSkill(skill->getName());
			if(getPP()>=skill->getCostPP()) {
				gui->setHighlightedPPAmount(skill->getCostPP());	
			}
		}
		else {
			gui->unChooseSkill(skill->getName());
		}

		int deltaPP = skill->getCostPP()-getPP();
		if(deltaPP < 0) deltaPP = 0;
		gui->setCounter(skill->getName(), std::to_string(deltaPP));
	}
}
	
void BattleScene::setPP(int value)
{
	if(value<0) value = 0;
	if(value>_maxPP) value = _maxPP;

	_pp = value;
}

void BattleScene::addPP(int value)
{
	setPP(_pp+value);
}

int BattleScene::getPP()
{
	return _pp;
}

void BattleScene::setMaxPP(int value)
{
	if(value<0) value = 0;
	_maxPP = value;
	setPP(_pp);
}

void BattleScene::addMaxPP(int value)
{
	setMaxPP(_maxPP + value);
}

int BattleScene::getMaxPP()
{
	return _maxPP;
}

void BattleScene::setBrokenPP(int value)
{
	if(value<0) value = 0;
	if(value>_maxPP) value = _maxPP;
	_brokenPP = value;
}

void BattleScene::addBrokenPP(int value)
{
	setBrokenPP(_brokenPP + value);
}

int BattleScene::getBrokenPP()
{
	return _brokenPP;
}

// ----------------- Main -----------------

void BattleScene::update(float deltaTime)
{
	// Change skill
	auto& input = _inputModule->getManager();
	int skillIndex = -1;
	if(input.isPressed(Keyboard::Q)) skillIndex = 0;
	if(input.isPressed(Keyboard::W)) skillIndex = 1;
	if(input.isPressed(Keyboard::E)) skillIndex = 2;
	if(input.isPressed(Keyboard::R)) skillIndex = 3;
	if(input.isPressed(Keyboard::T)) skillIndex = 4;
	if(input.isPressed(Keyboard::Y)) skillIndex = 5;

	if(skillIndex != -1 && skillIndex < _player->getSkillAmount()) {
		BattleSkill* nextSkill = _player->getSkill(skillIndex);

		if(nextSkill == _selectedSkill)
			_selectedSkill = nullptr;
		else
			_selectedSkill = nextSkill;
	}

	// Update selected skill
	if(_selectedSkill != nullptr) {
		_selectedSkill->selectedUpdate();
	}

	// Update pawns
	for(size_t i = 0; i<_pawns.size(); ++i) {
		_pawns[i]->update(deltaTime);
	}

	// Update effects
	for(size_t i = 0; i<_effects.size(); ++i) {
		_effects[i]->update();
	}

	// Update collisions
	updateCollision();

	// Remove sprites	
	for(auto itr = _sprites.begin(); itr != _sprites.end();) {

		if((*itr)->isAlive()) {
			++itr;
		}
		else {
			itr = _sprites.erase(itr);
		}
	}

	// Remove effects
	for(auto itr = _effects.begin(); itr != _effects.end();) {

		if((*itr)->isAlive()) {
			++itr;
		}
		else {
			itr = _effects.erase(itr);
		}
	}

	// Remove triggers
	for(auto itr = _triggers.begin(); itr != _triggers.end();) {

		if((*itr)->isAlive()) {
			++itr;
		}
		else {
			itr = _triggers.erase(itr);
		}
	}

	// Removing pawns
	updateRemovingPawns();
}

void BattleScene::render(sf3d::RenderTarget& target)
{
	for(auto& obj : _pawns) {
		obj->render(target);
	}

    target.draw(_areaShape);

	for(auto& obj : _pawns) {
		obj->bottomRender(target);
	}

	for(auto& obj : _pawns) {
		obj->renderSkills(target);
	}

	for(auto& obj : _sprites) {
		obj->render(target);
	}

	if(_selectedSkill != nullptr) 
		_selectedSkill->renderSpellIndicator();

	for(auto& obj : _pawns) {
		if(obj.get() != getPlayer()) {
			obj->renderBars(target);
		}
	}

	sf3d::CircleShape shape;
	shape.setRadius(20.f);
	auto pos = _battleModule->getCursorPosition(_position.y);
	shape.setPosition(pos);
	shape.setColor({200.f, 20.f, 180.f, 40.f});
	shape.setOrigin({50.f, 50.f, 0.f});
	target.draw(shape);

	updateGuiParameters();
}

// ----------------- Script -----------------

void BattleScene::initScript(Script& script)
{
	auto object = script.newClass<BattleScene>("BattleScene", "Battle");

	object.set("addPawn", &BattleScene::addPawn);
	object.set("addPlayer", &BattleScene::addPlayer);
	object.set("getPlayer", &BattleScene::getPlayer);
	object.set("activate", &BattleScene::activate);
	object.set("getCursorPosition", &BattleScene::getCursorPosition);

	object.set("setPP", &BattleScene::setPP);
	object.set("addPP", &BattleScene::addPP);
	object.set("getPP", &BattleScene::getPP);
	object.set("setMaxPP", &BattleScene::setMaxPP);
	object.set("addMaxPP", &BattleScene::addMaxPP);
	object.set("getMaxPP", &BattleScene::getMaxPP);
	object.set("setBrokenPP", &BattleScene::setBrokenPP);
	object.set("addBrokenPP", &BattleScene::addBrokenPP);
	object.set("getBrokenPP", &BattleScene::getBrokenPP);

	object.set("onFinish", &BattleScene::_onFinish);

	object.init();
}

} // namespace rat;