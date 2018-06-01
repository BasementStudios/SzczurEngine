#include "ScriptableComponent.hpp"

#include "../Entity.hpp"
#include "../ScenesManager.hpp"

#include <Szczur/Modules/Script/Script.hpp>

#include "Szczur/Utility/Convert/Windows1250.hpp"

namespace rat {
	ScriptableComponent::ScriptableComponent(Entity* parent) :
	Component { parent, fnv1a_64("ScriptableComponent"), "ScriptableComponent"} {

	}

	///
	void ScriptableComponent::update(ScenesManager& scenes, float deltaTime) {
		if(_updateCallback.valid()) {
			_updateCallback(this, deltaTime);
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
		else {
			_updateCallback = sol::function();
		}
	}

	/// Run any script for object
	void ScriptableComponent::loadAnyScript(const std::string& path) {
		auto& script = *detail::globalPtr<Script>;

		script.get()["THIS"] = getEntity();
		script.scriptFile(path);
		script.get()["THIS"] = sol::nil;
	}

	///
	void ScriptableComponent::loadFromConfig(const Json& config)
	{
		// Component::loadFromConfig(config);
		// auto& spriteDisplayDataHolder = getEntity()->getScene()->getSpriteDisplayDataHolder();
		// auto name = mapUtf8ToWindows1250(config["spriteDisplayData"].get<std::string>());
		// if(name != "") {
		// 	bool found{false};
		// 	for(auto& it : spriteDisplayDataHolder) {
		// 		if(name == it.getName()) {
		// 			setSpriteDisplayData(&it);
		// 			found = true;
		// 		}
		// 	}
		// 	if(!found) {
		// 		try {
		// 			setSpriteDisplayData(&(spriteDisplayDataHolder.emplace_back(name)));
		// 		}
		// 		catch(const std::exception& exc) {

		// 		}
		// 	}
		// }
	}

	///
	void ScriptableComponent::saveToConfig(Json& config) const
	{
		// Component::saveToConfig(config);
		// config["spriteDisplayData"] = _spriteDisplayData ? mapWindows1250ToUtf8(_spriteDisplayData->getName()) : "";
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
		object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));
		object.init();
	}

	void ScriptableComponent::renderHeader(ScenesManager& scenes, Entity* object) {
		if(ImGui::CollapsingHeader("Scriptable##scriptable_component")) {
			static bool onceType = false;

			// Load script button
			if(ImGui::Button("Load##scriptable_component")) {

				// Path to script
				std::string file = scenes.getRelativePathFromExplorer("Select script file", ".\\Assets", "Lua (*.lua)|*.lua");

				// Loading script
				if(file != "") {
					try {
						if(onceType) {
							loadAnyScript(file);
						}
						else {
							loadScript(file);
						}
					}
					catch(const std::exception& exc) {
						LOG_EXCEPTION(exc);
					}
				}
			}

			// Some options for selected script
			if(getFilePath()!="") {

				// Reload current script button
				ImGui::SameLine();
				if(ImGui::Button("Reload##scriptable_component")) {
					try {
						reloadScript();
					}
					catch (const std::exception& exc)
					{
						LOG_EXCEPTION(exc);
					}
				}

				// Remove script from object
				ImGui::SameLine();
				if(ImGui::Button("Remove##scriptable_component")) {
					loadScript("");
				}
			}

			// Show path to current script
			ImGui::Text("Path:");
			ImGui::SameLine();
			ImGui::Text(getFilePath()!="" ? mapWindows1250ToUtf8(getFilePath()).c_str() : "None");

			// Once type checkbox
			ImGui::Checkbox("Once type##scriptable_component", &onceType);
		}
	}

}
