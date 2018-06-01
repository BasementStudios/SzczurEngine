#pragma once

#include "../Component.hpp"

#include <Szczur/Modules/Script/Script.hpp>

#include <sol.hpp>

namespace rat {
class Entity;

class ScriptableComponent : public Component {
public:

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

	///
	void update(ScenesManager& scenes, float deltaTime);

	///
	virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

	/// Set script and run

	/// Set all values on default and remove script [unused]
	void reset();

	///
	const std::string& getFilePath();

// Scripts

	void loadScript(const std::string& path);

	/// Run script if is set
	void reloadScript();

	/// Run any script for object
	void loadAnyScript(const std::string& path);

	///
	virtual void loadFromConfig(Json& config) override;

	///
	virtual void saveToConfig(Json& config) const override;

	///
	static void initScript(Script& script);

	///
	virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

private:

	

	sol::function _updateCallback;
	std::string _scriptFilePath;
};
}