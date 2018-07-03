#include "LevelEditor.hpp"

#include <experimental/filesystem>

#include <imgui.h>
#include <glm/glm.hpp>

#include "../ScenesManager.hpp"
#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../Components/TraceComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/ArmatureComponent.hpp"
#include "../Components/ScriptableComponent.hpp"
#include "../Components/InteractableComponent.hpp"
#include "../Components/TriggerComponent.hpp"
#include "../Components/BaseComponent.hpp"
#include "../Components/AudioComponent.hpp"

#include "Szczur/Utility/Convert/Windows1250.hpp"

namespace rat {

	void LevelEditor::_renderProperties() {

		// Object isn't selected
		if(!_objectsList.isEntitySelected()) return;
		bool openWindow = true; 

		// Current scene
		auto* scene = _scenes.getCurrentScene();
		
		// Selected object
		Entity* focusedObject = _objectsList.getSelectedEntity();

		// Begin of properties window
		if(ImGui::Begin("Properties", &openWindow)) {

			// Change components
			if(focusedObject->getGroup() != "entries") {
				if(ImGui::Button("Change components...")) {
					ImGui::OpenPopup("Change components...##modal");
					ImGui::SetNextWindowSize(ImVec2(300,300));
				}
			}
			if(_scenes.isGameRunning()) { 
				ImGui::SameLine(); 
				if(ImGui::Button("Update status##properties")) { 
					_scenes.saveEntityToConfig(focusedObject, _scenes.getRunConfig()); 
					printMenuBarInfo(std::string("Updated status for: ")+focusedObject->getName()); 
				}
			}
			_renderComponentsManager();

			// Render headers for components
			if(focusedObject) {                
				if(auto* object = focusedObject->getComponentAs<BaseComponent>()) object->renderHeader(_scenes, focusedObject);
				if(auto* object = focusedObject->getComponentAs<SpriteComponent>()) object->renderHeader(_scenes, focusedObject);
				if(auto* object = focusedObject->getComponentAs<CameraComponent>()) object->renderHeader(_scenes, focusedObject);
				if(auto* object = focusedObject->getComponentAs<ArmatureComponent>()) object->renderHeader(_scenes, focusedObject);
				if(auto* object = focusedObject->getComponentAs<ColliderComponent>()) object->renderHeader(_scenes, focusedObject);
				if(auto* object = focusedObject->getComponentAs<ScriptableComponent>()) object->renderHeader(_scenes, focusedObject);
				if(auto* object = focusedObject->getComponentAs<InteractableComponent>()) object->renderHeader(_scenes, focusedObject);
				if(auto* object = focusedObject->getComponentAs<TriggerComponent>()) object->renderHeader(_scenes, focusedObject);
				if(auto* object = focusedObject->getComponentAs<TraceComponent>()) object->renderHeader(_scenes, focusedObject);
				if(auto* object = focusedObject->getComponentAs<AudioComponent>()) object->renderHeader(_scenes, focusedObject);
				if(auto* object = focusedObject->getComponentAs<BattleComponent>()) object->renderHeader(_scenes, focusedObject);

			}

		}
		ImGui::End();

		// Unselect object after close properties window
		if(openWindow == false) {
			_objectsList.unselect();
		}
	}

	void LevelEditor::_renderComponentsManager() {
		if (ImGui::BeginPopupModal("Change components...##modal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {

			auto* entity = _objectsList.getSelectedEntity();

			// Choosed components
			static bool choosed[7]{};
			if(ImGui::IsWindowAppearing()) {
				choosed[0] = entity->hasComponent<SpriteComponent>();
				choosed[1] = entity->hasComponent<ArmatureComponent>();
				choosed[2] = entity->hasComponent<ColliderComponent>();
				choosed[3] = entity->hasComponent<ScriptableComponent>();
				choosed[4] = entity->hasComponent<InteractableComponent>();
				choosed[5] = entity->hasComponent<TriggerComponent>();
				choosed[6] = entity->hasComponent<TraceComponent>();
				choosed[6] = entity->hasComponent<AudioComponent>();
			}


#define UPDATE_COMPONENT(NAME, ID) if(entity->hasComponent< NAME >() != choosed[ ID ]) { \
	if(choosed[ ID ]) entity->addComponent< NAME >(); else entity->removeComponent< NAME >(); \
}
			// Accept components button
			if(ImGui::Button("Accept", ImVec2(70,0))) {

				// Update components in object
				UPDATE_COMPONENT(SpriteComponent, 0);
				UPDATE_COMPONENT(ArmatureComponent, 1);
				UPDATE_COMPONENT(ColliderComponent, 2);
				UPDATE_COMPONENT(ScriptableComponent, 3);
				UPDATE_COMPONENT(InteractableComponent, 4);
				UPDATE_COMPONENT(TriggerComponent, 5);
				UPDATE_COMPONENT(TraceComponent, 6);
				UPDATE_COMPONENT(AudioComponent, 7);

				ImGui::CloseCurrentPopup();
			}
#undef UPDATE_COMPONENT

			// Close popup button
			ImGui::SameLine();
			if(ImGui::Button("Close", ImVec2(70,0))) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::Checkbox("Sprite", &choosed[0]);
			ImGui::Checkbox("Armature", &choosed[1]);
			ImGui::Checkbox("Collider", &choosed[2]);
			ImGui::Checkbox("Scriptable", &choosed[3]);
			ImGui::Checkbox("Interactable", &choosed[4]);
			ImGui::Checkbox("Trigger", &choosed[5]);
			ImGui::Checkbox("Trace", &choosed[6]);
			ImGui::Checkbox("Audio", &choosed[7]);

			ImGui::EndPopup();
		}
	}
}
