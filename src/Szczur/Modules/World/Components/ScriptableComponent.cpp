#include "ScriptableComponent.hpp"

#include <nlohmann/json.hpp>

#include "../Entity.hpp"
#include "../ScenesManager.hpp"

#include <Szczur/Modules/Script/Script.hpp>

#include "Szczur/Utility/Convert/Windows1250.hpp"
#include "Szczur/Modules/Script/Script.hpp"

#ifdef OS_WINDOWS
#include <shellapi.h>
#endif

namespace rat {

// ========== Constructors ==========

	ScriptableComponent::ScriptableComponent(Entity* parent) :
	Component { parent, fnv1a_64("ScriptableComponent"), "ScriptableComponent"} {

	}

// ========== Main ==========

	void ScriptableComponent::update(ScenesManager& scenes, float deltaTime) {
		if(_inited) {
			if(_updateCallback.valid()) {
				_updateCallback(getEntity(), deltaTime);
			}      
		}
		else {      
			_inited = true;

			try
			{
				if (_initCallback.valid())
				{
					_initCallback(getEntity());
				}
			}
			catch (sol::error e)
			{
				LOG_EXCEPTION(e);
			}
		}
	}

	void ScriptableComponent::sceneChanged() {		
		try
		{
			if (_sceneChangeCallback.valid())
			{
				_sceneChangeCallback(getEntity());
			}
		}
		catch (sol::error e)
		{
			LOG_EXCEPTION(e);
		}
	}

	void ScriptableComponent::callInit()
	{
		_inited = false;
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
		try {
			if(path != "") {
				auto& script = *detail::globalPtr<Script>;

				script.get()["THIS"] = getEntity();
				script.scriptFile(path);
				script.get()["THIS"] = sol::nil;
			}
		}
		catch(sol::error e) {
			LOG_EXCEPTION(e);
		}
	}

	///
	void ScriptableComponent::loadFromConfig(nlohmann::json& config)
	{
		Component::loadFromConfig(config);
		if(auto& var = config["script"]; !var.is_null()) _scriptPath = var.get<std::string>();
	}

	///
	void ScriptableComponent::saveToConfig(nlohmann::json& config) const
	{
		Component::saveToConfig(config);
		config["script"] = _scriptPath;
	}

	void ScriptableComponent::initScript(ScriptClass<Entity>& entity, Script& script)
	{
		auto object = script.newClass<ScriptableComponent>("ScriptableComponent", "World");

		// Main
		object.set("getFilePath", &ScriptableComponent::getFilePath);
		object.set("runScript", sol::resolve<void(const std::string&)>(&ScriptableComponent::runScript));
		object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));

		// Entity
		entity.set("addScriptableComponent", [&](Entity& e){return (ScriptableComponent*)e.addComponent<ScriptableComponent>();});
		entity.setProperty("onUpdate", [](){}, [](Entity &obj, sol::function func) {
			obj.getComponentAs<ScriptableComponent>()->_updateCallback = func;
		});
		entity.setProperty("onInit", [](){}, [](Entity &obj, sol::function func) {
			obj.getComponentAs<ScriptableComponent>()->_initCallback = func;
		});
		entity.setProperty("onChangeScene", [](){}, [](Entity &obj, sol::function func) {
			obj.getComponentAs<ScriptableComponent>()->_sceneChangeCallback = func;
		});
		entity.set("scriptable", &Entity::getComponentAs<ScriptableComponent>);

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
					std::string file = scenes.getRelativePathFromExplorer("Select script file", ".\\Assets", "Lua (*.lua)|*.lua");
					runScript(file);
				}
			
			}

			// Show path to current script
			ImGui::Text("Path:");
			ImGui::SameLine();
			
			if (getFilePath().empty())
			{
				ImGui::Text("None");
			}
			else
			{
				ImGui::Text(mapWindows1250ToUtf8(getFilePath()).c_str());

				if (ImGui::Button("Open file in editor"))
				{
#ifdef OS_WINDOWS
					ShellExecuteA(NULL, "open", getFilePath().c_str(), NULL, NULL, SW_SHOWDEFAULT);
#endif
				}

				ImGui::SameLine();

				if (ImGui::Button("Open cotaining folder"))
				{
					std::experimental::filesystem::path path = mapWindows1250ToUtf8(getFilePath());

#ifdef OS_WINDOWS
					ShellExecuteA(NULL, "open", path.parent_path().string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
#endif
				}
			}
		}
	}

}
