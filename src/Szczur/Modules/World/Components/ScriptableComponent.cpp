#include "ScriptableComponent.hpp"

#include "../Entity.hpp"

#include <Szczur/Modules/Script/Script.hpp>

namespace rat {
	ScriptableComponent::ScriptableComponent(Entity* parent) :
	Component { parent, fnv1a_64("ScriptableComponent"), "ScriptableComponent"} {

	}   

	///
	void ScriptableComponent::init() {
		if(_initCallback.valid())
			_initCallback(getEntity());
	}

	///
	void ScriptableComponent::update(float deltaTime) {
		if(_initCallback.valid())
			_updateCallback(getEntity(), deltaTime);
	}

	/// Set script and run
	void ScriptableComponent::loadScript(const std::string& path) {
		_scriptFilePath = path;
		reloadScript();
	}

	/// Run script if is set
	void ScriptableComponent::reloadScript() {
		if(_scriptFilePath != "") {
			auto& script = *detail::globalPtr<Script>;

			script.get()["THIS"] = getEntity();
			script.scriptFile(_scriptFilePath);
			script.get()["THIS"] = sol::nil;
		}
	}

	/// Run any script for object
	void ScriptableComponent::loadAnyScript(const std::string& path) {
		auto& script = *detail::globalPtr<Script>;

		script.get()["THIS"] = getEntity();
		script.scriptFile(path);
		script.get()["THIS"] = sol::nil;
	}

	/// Set all values on default and remove script
	void ScriptableComponent::reset() {
		_initCallback = sol::unsafe_function();
		_updateCallback = sol::unsafe_function();
	}

}