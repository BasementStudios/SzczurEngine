#pragma once

#include <memory>

#include <Szczur/Modules/World/Component.hpp>

#include <Szczur/Modules/Battle/BattleConfig.hpp>

#include <Szczur/Utility/SFML3D/RectangleShape.hpp>

namespace rat
{

class Battle;
class Script;

class Entity;
template<class T> class ScriptClass;

class BattleComponent : public Component
{
public:

// Constructors

	///
	BattleComponent(Entity* parent);

	///
	~BattleComponent();

// Getters


// Saving and loading

	///
	virtual void loadFromConfig(Json& config);

	///
	virtual void saveToConfig(Json& config) const;

	///
	virtual std::unique_ptr<Component> copy(Entity* newParent) const;

// Manipulations

	void setSize(const glm::vec2& size);

// Scripts

	///
	static void initScript(ScriptClass<Entity>& entity, Script& script);

// Main

	///
	void update(ScenesManager& scenes, float deltaTime);

	///
	void render(sf3d::RenderTarget& target);

	///
	virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

private:

// Vars

	Battle* _battleModule;
	BattleConfig _config;
	sf3d::RectangleShape _areaShape;
};

}
