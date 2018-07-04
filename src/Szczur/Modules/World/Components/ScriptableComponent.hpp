#pragma once

#include "../Component.hpp"

#include <Szczur/Modules/Script/Script.hpp>

#include <sol2/sol.hpp>

namespace rat {

class Entity;
class Script;
template<class T> class ScriptClass;

class ScriptableComponent : public Component {
public:

// Constructors

	///
	ScriptableComponent(Entity* parent);

	///
	ScriptableComponent(const ScriptableComponent&) = default;

	///
	ScriptableComponent& operator = (const ScriptableComponent&) = default;

	///
	ScriptableComponent(ScriptableComponent&&) = default;

	///
	ScriptableComponent& operator = (ScriptableComponent&&) = default;

	///
	~ScriptableComponent() = default;

// Main

	///
	void update(ScenesManager& scenes, float deltaTime);

	///
	void sceneChanged();

	///
	virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

// Modifications

	///
	const std::string& getFilePath();

	///
	void setScriptPath(const std::string& path);

// Scripts

	/// Run script if is set
	void runScript();

	/// Run any script for object
	void runScript(const std::string& path);

	///
	virtual void loadFromConfig(Json& config) override;

	///
	virtual void saveToConfig(Json& config) const override;

	///
	static void initScript(ScriptClass<Entity>& entity, Script& script);

	///
	virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

private:

	sol::function _updateCallback;
	sol::function _initCallback;
	sol::function _sceneChangeCallback;
	bool _inited = false;
	std::string _scriptPath;
};
}
