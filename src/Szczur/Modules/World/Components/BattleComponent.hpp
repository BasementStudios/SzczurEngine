#pragma once

#include <memory>

#include <nlohmann/json_fwd.hpp>

#include <Szczur/Modules/World/Component.hpp>

#include <Szczur/Utility/SFML3D/Geometry/RectangleShape.hpp>

using Json = nlohmann::json;

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
	sf3d::RectangleShape _areaShape;
};

}
