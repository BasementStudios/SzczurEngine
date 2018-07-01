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

#include "Szczur/Utility/Convert/Windows1250.hpp"

namespace rat {

	void LevelEditor::_renderProperties() {

		bool isSingleSelected = _objectsList.isAnySingleEntitySelected();
		bool isGroupSelected = _objectsList.isGroupSelected();

		// Object isn't selected
		if (!isSingleSelected && !isGroupSelected)
			return;

		bool openWindow = true; 

		// Begin of properties window
		if(ImGui::Begin("Properties", &openWindow)) {

			if (isSingleSelected) {
				_renderSingleProperty();
			}
			else if (isGroupSelected) {
				_renderGroupProperty();
			}
		}
		ImGui::End();

		// Unselect object after close properties window
		if(openWindow == false) {
			_objectsList.unselect();
			_objectsList.clearSelected();
		}
	}

	void LevelEditor::_renderComponentsManager() {
		if (ImGui::BeginPopupModal("Change components...##modal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {

			auto* entity = _objectsList.getSelectedEntity();

			// Choosed components
			static bool choosed[8]{};
			if(ImGui::IsWindowAppearing()) {
				choosed[0] = entity->hasComponent<SpriteComponent>();
				choosed[1] = entity->hasComponent<AnimatedSpriteComponent>();
				choosed[2] = entity->hasComponent<ArmatureComponent>();
				choosed[3] = entity->hasComponent<ColliderComponent>();
				choosed[4] = entity->hasComponent<ScriptableComponent>();
				choosed[5] = entity->hasComponent<InteractableComponent>();
				choosed[6] = entity->hasComponent<TriggerComponent>();
				choosed[7] = entity->hasComponent<TraceComponent>();
			}


#define UPDATE_COMPONENT(NAME, ID) if(entity->hasComponent< NAME >() != choosed[ ID ]) { \
	if(choosed[ ID ]) entity->addComponent< NAME >(); else entity->removeComponent< NAME >(); \
}
			// Accept components button
			if(ImGui::Button("Accept", ImVec2(70,0))) {

				// Update components in object
				UPDATE_COMPONENT(SpriteComponent, 0);
				UPDATE_COMPONENT(AnimatedSpriteComponent, 1);
				UPDATE_COMPONENT(ArmatureComponent, 2);
				UPDATE_COMPONENT(ColliderComponent, 3);
				UPDATE_COMPONENT(ScriptableComponent, 4);
				UPDATE_COMPONENT(InteractableComponent, 5);
				UPDATE_COMPONENT(TriggerComponent, 6);
				UPDATE_COMPONENT(TraceComponent, 7);

				ImGui::CloseCurrentPopup();
			}
#undef UPDATE_COMPONENT

			// Close popup button
			ImGui::SameLine();
			if(ImGui::Button("Close", ImVec2(70,0))) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::Checkbox("Sprite", &choosed[0]);
			ImGui::Checkbox("Animated Sprite", &choosed[1]);
			ImGui::Checkbox("Armature", &choosed[2]);
			ImGui::Checkbox("Collider", &choosed[3]);
			ImGui::Checkbox("Scriptable", &choosed[4]);
			ImGui::Checkbox("Interactable", &choosed[5]);
			ImGui::Checkbox("Trigger", &choosed[6]);
			ImGui::Checkbox("Trace", &choosed[7]);

			ImGui::EndPopup();
		}
	}

	void LevelEditor::_renderSingleProperty() {
		// Selected object
		Entity* focusedObject = _objectsList.getSelectedEntity();

		// Change components
		if (focusedObject->getGroup() != "entries") {
			if (ImGui::Button("Change components...")) {
				ImGui::OpenPopup("Change components...##modal");
				ImGui::SetNextWindowSize(ImVec2(300, 300));
			}
		}
		if (_scenes.isGameRunning()) {
			ImGui::SameLine();
			if (ImGui::Button("Update status##properties")) {
				_scenes.saveEntityToConfig(focusedObject, _scenes.getRunConfig());
				printMenuBarInfo(std::string("Updated status for: ") + focusedObject->getName());
			}
		}
		_renderComponentsManager();

		// Render headers for components
		if (focusedObject) {
			if (auto* object = focusedObject->getComponentAs<BaseComponent>()) object->renderHeader(_scenes, focusedObject);
			if (auto* object = focusedObject->getComponentAs<SpriteComponent>()) object->renderHeader(_scenes, focusedObject);
			if (auto* object = focusedObject->getComponentAs<AnimatedSpriteComponent>()) object->renderHeader(_scenes, focusedObject);
			if (auto* object = focusedObject->getComponentAs<CameraComponent>()) object->renderHeader(_scenes, focusedObject);
			if (auto* object = focusedObject->getComponentAs<ArmatureComponent>()) object->renderHeader(_scenes, focusedObject);
			if (auto* object = focusedObject->getComponentAs<ColliderComponent>()) object->renderHeader(_scenes, focusedObject);
			if (auto* object = focusedObject->getComponentAs<ScriptableComponent>()) object->renderHeader(_scenes, focusedObject);
			if (auto* object = focusedObject->getComponentAs<InteractableComponent>()) object->renderHeader(_scenes, focusedObject);
			if (auto* object = focusedObject->getComponentAs<TriggerComponent>()) object->renderHeader(_scenes, focusedObject);
			if (auto* object = focusedObject->getComponentAs<TraceComponent>()) object->renderHeader(_scenes, focusedObject);

		}

	}

	void LevelEditor::_renderGroupProperty() {
		static glm::vec3 currentOffset;
		static glm::vec3 lastOffset;

		ImGui::Text("Selected: %d", _objectsList.getSelectedEntities().size());

		if (ImGui::DragFloat3("Offset", &currentOffset[0])) {
			auto delta = currentOffset - lastOffset;

			for (auto entity : _objectsList.getSelectedEntities())
			{
				entity->move(delta);
			}

			lastOffset = currentOffset;
		}

		if (ImGui::TreeNodeEx("Selected entities", ImGuiTreeNodeFlags_DefaultOpen)) {
			for (auto entity : _objectsList.getSelectedEntities())
			{
				ImGui::BulletText("%s", entity->getName().c_str());
			}

			ImGui::TreePop();
		}
	}
}
