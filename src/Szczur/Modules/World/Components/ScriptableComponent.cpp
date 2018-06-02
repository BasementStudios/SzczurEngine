#include "ScriptableComponent.hpp"

#include "../Entity.hpp"
#include "../ScenesManager.hpp"

#include <Szczur/Modules/Script/Script.hpp>

#include "Szczur/Utility/Convert/Windows1250.hpp"

namespace rat {

// ========== Constructors ==========

	ScriptableComponent::ScriptableComponent(Entity* parent) :
	Component { parent, fnv1a_64("ScriptableComponent"), "ScriptableComponent"} {

	}

// ========== Main ==========

	void ScriptableComponent::update(ScenesManager& scenes, float deltaTime) {
		if(!_inited) {			
			_inited = true;
			if(_initCallback.valid()) {
				_initCallback(this);
			}
		}
		if(_updateCallback.valid()) {
			_updateCallback(this, deltaTime);
		}
	}

	void ScriptableComponent::sceneChanged() {		
		if(_sceneChangeCallback.valid()) {
			_sceneChangeCallback(this);
		}
	}

	std::unique_ptr<Component> ScriptableComponent::copy(Entity* newParent) const
	{
		auto ptr = std::make_unique<ScriptableComponent>(*this);

		ptr->setEntity(newParent);
		ptr->_scriptPath = _scriptPath;
		ptr->_updateCallback = _updateCallback;

		return ptr;
	}

// ========== Modifications ==========

	void ScriptableComponent::setScriptPath(const std::string& path) {
		_scriptPath = path;
	}

	const std::string& ScriptableComponent::getFilePath() {
		return _scriptPath;
	}

// ========== Scripts ==========

	/// Run script if is set
	void ScriptableComponent::runScript() {
		runScript(_scriptPath);
	}

	/// Run any script for object
	void ScriptableComponent::runScript(const std::string& path) {
		if(path != "") {
			auto& script = *detail::globalPtr<Script>;

			script.get()["THIS"] = getEntity();
			script.scriptFile(path);
			script.get()["THIS"] = sol::nil;
		}
	}

	///
	void ScriptableComponent::loadFromConfig(Json& config)
	{
		Component::loadFromConfig(config);
		if(auto& var = config["script"]; !var.is_null()) _scriptPath = var.get<std::string>();
	}

	///
	void ScriptableComponent::saveToConfig(Json& config) const
	{
		Component::saveToConfig(config);
		config["script"] = _scriptPath;
	}

	void ScriptableComponent::initScript(Script& script) {
		auto object = script.newClass<ScriptableComponent>("ScriptableComponent", "World");
		// object.set("onUpdate", [](ScriptableComponent& obj){return "SCRIPT!";});
		object.set("onUpdate", &ScriptableComponent::_updateCallback);
		object.set("onInit", &ScriptableComponent::_initCallback);
		object.set("onSceneChange", &ScriptableComponent::_sceneChangeCallback);
		object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));
		object.init();
	}

	void ScriptableComponent::renderHeader(ScenesManager& scenes, Entity* object) {
		if(ImGui::CollapsingHeader("Scriptable##scriptable_component")) {

			// Load script button
			if(ImGui::Button("Load##scriptable_component")) {

				// Path to script
				std::string file = scenes.getRelativePathFromExplorer("Select script file", ".\\Assets", "Lua (*.lua)|*.lua");
				setScriptPath(file);
				if(scenes.isGameRunning()) runScript();
			}

			// Some options for selected script
			if(getFilePath()!="") {
				// Reload script for object
				if(scenes.isGameRunning()) {
					ImGui::SameLine();
					if(ImGui::Button("Reload##scriptable_component")) {
						runScript();
					}					
				}
				// Remove script from object
				ImGui::SameLine();
				if(ImGui::Button("Remove##scriptable_component")) {
					setScriptPath("");
				}
			}

			if(scenes.isGameRunning()) {
				ImGui::SameLine();
				if(ImGui::Button("Load once##scriptable_component")) {
					std::string file = scenes.getRelativePathFromExplorer("Select script file", ".\\Assets");
					runScript(file);
				}
			
			}

			// Show path to current script
			ImGui::Text("Path:");
			ImGui::SameLine();
			ImGui::Text(getFilePath()!="" ? mapWindows1250ToUtf8(getFilePath()).c_str() : "None");
		}
	}

}
