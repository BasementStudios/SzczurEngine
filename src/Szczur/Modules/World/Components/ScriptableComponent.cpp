#include "ScriptableComponent.hpp"

#include "../Entity.hpp"

#include <Szczur/Modules/Script/Script.hpp>

namespace rat {
	ScriptableComponent::ScriptableComponent(Entity* parent) :
	Component { parent, fnv1a_64("ScriptableComponent"), "ScriptableComponent"} {

	}   

	///
	void ScriptableComponent::update(float deltaTime) {
		if(_updateCallback.valid()) {
			_updateCallback(getEntity(), deltaTime);
		}
	}

	std::unique_ptr<Component> ScriptableComponent::copy(Entity* newParent) const
	{
		auto ptr = std::make_unique<ScriptableComponent>(*this);

        ptr->setEntity(newParent);
        ptr->_scriptFilePath = _scriptFilePath;
        ptr->_updateCallback = _updateCallback;

        return ptr;
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
		_updateCallback = sol::function();
	}

	const std::string& ScriptableComponent::getFilePath() {
		return _scriptFilePath;
	}

	void ScriptableComponent::initScript(Script& script) {
		auto object = script.newClass<ScriptableComponent>("ScriptableComponent", "World");
		// object.set("onUpdate", [](ScriptableComponent& obj){return "SCRIPT!";});
		object.set("onUpdate", &ScriptableComponent::_updateCallback);
		object.init();
	}
}