#include "BattleComponent.hpp"

#include "Szczur/Modules/DragonBones/SF3DFactory.hpp"

#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../ScenesManager.hpp"

#include <Szczur/Modules/Script/Script.hpp>
#include <Szczur/Modules/Battle/Battle.hpp>
#include <Szczur/Modules/Battle/BattleScene.hpp>

namespace rat {

// ----------------- Constructors -----------------

BattleComponent::BattleComponent(Entity* parent) :
	Component(parent, fnv1a_64("BattleComponent"), "BattleComponent")
{
	_battleModule = detail::globalPtr<Battle>;

	setSize(glm::vec2(800.f, 400.f));
   	_areaShape.setColor({ 0.1f, 0.50f, 0.70f, 0.3f }); 
   	_areaShape.setRotation({ 90.f, 0.f, 0.f });

	// TEST
	// _config.addPawn("pig")->position = glm::vec2(400.f, 200.f);
	// _config.addPawn("pig")->position = glm::vec2(0.f, 0.f);
	// --TEST
}

BattleComponent::~BattleComponent() 
{

}

// ----------------- Getters -----------------

std::unique_ptr<Component> BattleComponent::BattleComponent::copy(Entity* newParent) const
{	
	auto ptr = std::make_unique<BattleComponent>(*this);

	// Coping here...

	ptr->setEntity(newParent);

	return ptr;
}

// ----------------- Saving and loading -----------------

void BattleComponent::loadFromConfig(Json& config) 
{
	Component::loadFromConfig(config);
}

void BattleComponent::saveToConfig(Json& config) const 
{
	Component::saveToConfig(config);
}

// ----------------- Manipulations -----------------

void BattleComponent::setSize(const glm::vec2& size)
{
	// _config.setSize(size);
   	_areaShape.setSize(size); 
   	_areaShape.setOrigin({ size.x / 2.f, size.y / 2.f, 0.f }); 
}

// ----------------- Scripts -----------------

void BattleComponent::initScript(ScriptClass<Entity>& entity, Script& script) 
{
	auto object = script.newClass<ArmatureComponent>("BattleComponent", "World");
	// Script here...
	object.init();
}

// ----------------- Main -----------------

void BattleComponent::update(ScenesManager& scenes, float deltaTime) 
{

}

void BattleComponent::render(sf3d::RenderTarget& target) 
{
   	_areaShape.setPosition(getEntity()->getPosition()); 
    // target.draw(_areaShape);
}

void BattleComponent::renderHeader(ScenesManager& scenes, Entity* object) 
{
	// if(ImGui::CollapsingHeader("Battle##battle_component")) {

	// 	// Edit battle button
	// 	if(ImGui::Button("Edit battle")) {
	// 		_config.setPosition(getEntity()->getPosition());
	// 		auto* scene = _battleModule->activateScene(getEntity()->getScene());
	// 		scene->setupConfig(&_config);
	// 	}

	// 	// Change area size
	// 	glm::vec2 areaSize = _config.getSize();
	// 	if(ImGui::DragFloat2("Area size", reinterpret_cast<float*>(&areaSize))) {
		
	// 	}
	// 	setSize(areaSize);
	// }
}

} //namespace rat
