#include "Battle.hpp"

#include "BattleScene.hpp"
#include "BattlePawn.hpp"
#include "BattleSkill.hpp"
#include "BattleEffect.hpp"
#include "BattleTrigger.hpp"
#include "BattleSprite.hpp"
#include "BattleAnimationSprite.hpp"

#include <Szczur/Utility/SFML3D/Geometry/Linear.hpp>

#include <Szczur/Modules/World/World.hpp>
#include <Szczur/Modules/World/Scene.hpp>
#include <Szczur/Modules/Script/Script.hpp>

namespace rat {

// ----------------- Constructors -----------------

Battle::Battle()
{
    LOG_INFO("Initializing Battle module");

	const auto& videoMode = getModule<Window>().getVideoMode();
	_defaultWindowSize = glm::vec2(videoMode.width, videoMode.height);

    initScript();
    _pawnManager.loadAllPawns("Assets/Pawns/pawns.json");
    _textureManager.loadAllTextures("Assets/Battles/textures/textures.json");

    LOG_INFO("Module Battle initialized");

    // TEST
    getModule<Script>().scriptFile("Assets/Battles/test_battle.lua");
    getPlayer()->setSkills({"Move", "Stun hit", "Dash and hit", "Magic bomb", "Explosion"});
	// --TEST
}

Battle::~Battle()
{
    LOG_INFO("Module Battle destructed");
}

// ----------------- Pawn manager -----------------

BattlePawnManager& Battle::getPawnManager()
{
	return _pawnManager;
}

// ----------------- Spell indicator -----------------

BattleSpellIndicatorManager& Battle::getSpellIndicatorManager()
{
	return _spellIndicatorManager;
}

// ----------------- Spell indicator -----------------

BattleTextureManager& Battle::getTextureManager()
{
	return _textureManager;
}

// ----------------- Effects -----------------

BattleEffect* Battle::newEffect()
{
	if(isActiveScene()) {
		return getCurrentScene()->newEffect();
	}
	return nullptr;
}

BattleEffect* Battle::newEffect(BattleSkill* skill)
{
	BattleEffect* ret = newEffect();
	ret->setSkill(skill);
	return ret;
}

// ----------------- Triggers -----------------

BattleTrigger* Battle::newTrigger()
{
	if(isActiveScene()) {
		return getCurrentScene()->newTrigger();
	}
	return nullptr;
}

// ----------------- Pawns -----------------

BattlePawn* Battle::getPlayer()
{
	if(isActiveScene()) {
		return getCurrentScene()->getPlayer();
	}
	return nullptr;
}

void Battle::setPlayerSkills(const std::vector<std::string>& _skillNames)
{
	if(isActiveScene()) {
		getPlayer()->setSkills(_skillNames);
	}
}

// ----------------- Sprites -----------------

BattleSprite* Battle::newSprite(const std::string& textureName)
{
	if(isActiveScene()) {
		return getCurrentScene()->newSprite(textureName);
	}
	return nullptr;
}

BattleAnimationSprite* Battle::newAnimationSprite(const std::string& textureName)
{
	if(isActiveScene()) {
		return getCurrentScene()->newAnimationSprite(textureName);
	}
	return nullptr;
}

// ----------------- Scene -----------------

BattleScene* Battle::createScene(const glm::vec3& position, const glm::vec2& size, float scale)
{
	getModule<Player>().start();
	
	_currentScene.reset(new BattleScene(position, size, scale));

	return _currentScene.get();
}

void Battle::deactivateScene() 
{
	getModule<Player>().stop();

	if(isActiveScene()) {
		_currentScene->finish();
		_currentScene = nullptr;
		_battleActive = false;
	}
}

bool Battle::isActiveScene()
{
	return _currentScene.get() != nullptr && _currentScene->isActive();
}

BattleScene* Battle::getCurrentScene()
{
	return _currentScene.get();
}

// ----------------- Mouse -----------------

bool Battle::isSkillButtonPressed()
{
	return getModule<Input>().getManager().isPressed(Mouse::Left);
}

bool Battle::isSkillButtonKept()
{
	return getModule<Input>().getManager().isKept(Mouse::Left);
}

bool Battle::isSkillButtonReleased()
{
	return getModule<Input>().getManager().isReleased(Mouse::Left);
}

glm::vec3 Battle::getCursorPosition(float height)
{
	// Mouse position
	auto mouse = getModule<Input>().getManager().getScreenMousePosition();

	// Projection on Y-plane
	auto linear = getModule<Window>().getWindow().getLinearByScreenPosition(mouse);
	glm::vec3 projection = linear.getCameraProjectionY(height);

	return projection;
}

glm::vec2 Battle::getCursorPosition()
{
	if(isActiveScene()) {
		glm::vec3 pos = getCursorPosition(getCurrentScene()->getPosition().y);
		return glm::vec2(pos.x, pos.z);
	}

	return glm::vec2();
}

// ----------------- Math -----------------

glm::vec2 Battle::getMovedPosition(glm::vec2 position, float angle, float distance)
{
	position.x += std::cos(angle)*distance;
	position.y += std::sin(angle)*distance;
	return position;
}

bool Battle::checkCollisionCC(glm::vec2 pos1, float r1, glm::vec2 pos2, float r2)
{
	float dis = glm::distance(pos1, pos2);

	return dis < r1+r2;
}

glm::vec2 Battle::getNormalDirection(glm::vec2 from, glm::vec2 to)
{
	return glm::normalize(to-from);
}

float Battle::getDistance(glm::vec2 pos1, glm::vec2 pos2)
{
	return glm::distance(pos1, pos2);
}

// ----------------- Main -----------------

void Battle::render(sf3d::RenderTarget& target)
{
	if(isActiveScene()) {
		getSpellIndicatorManager().setRenderTarget(target);
		getSpellIndicatorManager().setBattleScene(_currentScene.get());
		_currentScene->render(target);
	}
}

void Battle::update(float deltaTime)
{
	if(isActiveScene()) {

		// TEST
		if(detail::globalPtr<Input>->getManager().isPressed(Keyboard::Escape)) {
			deactivateScene();
		}
		// --TEST

		if(_battleActive == false) {
			_battleActive = true;
			// set Player invisible
			// set Player inactive 
		}
		_currentScene->update(deltaTime);
		if(getPlayer()->getHealth()<=0) {
			deactivateScene();
		}
		else if(getCurrentScene()->getPawns().size()==1) {
			deactivateScene();
		}
	}
}

// ----------------- Script -----------------

void Battle::initScript()
{
	Script& script = getModule<Script>();

	auto module = script.newModule("Battle");

	// Effects
	module.set_function("newEffect", sol::overload(
		[&](BattleSkill* skill){return newEffect(skill);},
		[&](){return newEffect();}
	));

	// Triggers
	module.set_function("newTrigger", &Battle::newTrigger, this);

	// Pawns
	module.set_function("getPlayer", &Battle::getPlayer, this);

	// Sprites
	module.set_function("newSprite", &Battle::newSprite, this);
	module.set_function("newAnimationSprite", &Battle::newAnimationSprite, this);

	// Scene
	module.set_function("createScene", &Battle::createScene, this);
	module.set_function("deactivateScene", &Battle::deactivateScene, this);
	module.set_function("isActiveScene", &Battle::isActiveScene, this);
	module.set_function("getCurrentScene", &Battle::getCurrentScene, this);

	// Mouse
	module.set_function("isSkillButtonPressed", &Battle::isSkillButtonPressed, this);
	module.set_function("isSkillButtonKept", &Battle::isSkillButtonKept, this);
	module.set_function("isSkillButtonReleased", &Battle::isSkillButtonReleased, this);

	// Math
	module.set_function("getMovedPosition", &Battle::getMovedPosition, this);
	module.set_function("checkCollisionCC", &Battle::checkCollisionCC, this);
	module.set_function("getNormalDirection", &Battle::getNormalDirection, this);
	module.set_function("getDistance", &Battle::getDistance, this);

	// Cursor
	module.set_function("getCursorPosition", [&]() {
		return getCursorPosition();
	});

	script.initClasses<BattleScene, BattlePawn, BattleSkill, BattleEffect, BattleTrigger, BattleSprite, BattleAnimationSprite, BattleSpellIndicatorManager>();
}

} // namespace rat
