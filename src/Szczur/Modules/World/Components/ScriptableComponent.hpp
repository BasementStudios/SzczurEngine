#pragma once

#include <sol.hpp>
#include <nlohmann/json_fwd.hpp>

namespace rat {
	class Entity;
	class Script;
	template<class T> class ScriptClass;
}
#include "Szczur/Modules/Script/Script.hpp"
#include "../Component.hpp"

namespace rat
{

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
	void callInit();

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
	virtual void loadFromConfig(nlohmann::json& config) override;

	///
	virtual void saveToConfig(nlohmann::json& config) const override;

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
