#include "LevelEditor.hpp"

#include <experimental/filesystem>

#include <imgui.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "../ScenesManager.hpp"
#include "../Entity.hpp"
#include "../Scene.hpp"
#include "../Components/BaseComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/CameraComponent.hpp"
#include "../Components/ArmatureComponent.hpp"
#include "../Components/ColliderComponent.hpp"
#include "../Components/ScriptableComponent.hpp"
#include "../Components/InteractableComponent.hpp"
#include "../Components/TriggerComponent.hpp"
#include "../Components/TraceComponent.hpp"
#include "../Components/PointLightComponent.hpp"

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
		}
	}

	void LevelEditor::_renderComponentsManager() {
		if (ImGui::BeginPopupModal("Change components...##modal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {

			auto* entity = _objectsList.getSelectedEntity();

			// Choosed components
			static bool choosed[16]{};
			if(ImGui::IsWindowAppearing()) {
				choosed[0] = entity->hasComponent<SpriteComponent>();
				choosed[1] = entity->hasComponent<AnimatedSpriteComponent>();
				choosed[2] = entity->hasComponent<ArmatureComponent>();
				choosed[3] = entity->hasComponent<ColliderComponent>();
				choosed[4] = entity->hasComponent<ScriptableComponent>();
				choosed[5] = entity->hasComponent<InteractableComponent>();
				choosed[6] = entity->hasComponent<TriggerComponent>();
				choosed[7] = entity->hasComponent<TraceComponent>();
				choosed[8] = entity->hasComponent<PointLightComponent>();
			}

#define UPDATE_COMPONENT(NAME, ID) if(entity->hasComponent< NAME >() != choosed[ ID ]) { \
	if(choosed[ ID ]) entity->addComponent< NAME >(); else entity->removeComponent< NAME >(); \
}
			// Accept components button
			if(ImGui::Button("Accept", ImVec2(70,0))) {

				// Update components in object
				UPDATE_COMPONENT(SpriteComponent, 			0);
				UPDATE_COMPONENT(AnimatedSpriteComponent, 	1);
				UPDATE_COMPONENT(ArmatureComponent, 		2);
				UPDATE_COMPONENT(ColliderComponent, 		3);
				UPDATE_COMPONENT(ScriptableComponent, 		4);
				UPDATE_COMPONENT(InteractableComponent, 	5);
				UPDATE_COMPONENT(TriggerComponent, 			6);
				UPDATE_COMPONENT(TraceComponent, 			7);
				UPDATE_COMPONENT(PointLightComponent, 		8);

				ImGui::CloseCurrentPopup();
			}
#undef UPDATE_COMPONENT

			// Close popup button
			ImGui::SameLine();
			if(ImGui::Button("Close", ImVec2(70,0))) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::Checkbox("Sprite", 			&choosed[0]);
			ImGui::Checkbox("Animated Sprite", 	&choosed[1]);
			ImGui::Checkbox("Armature", 		&choosed[2]);
			ImGui::Checkbox("Collider", 		&choosed[3]);
			ImGui::Checkbox("Scriptable", 		&choosed[4]);
			ImGui::Checkbox("Interactable", 	&choosed[5]);
			ImGui::Checkbox("Trigger", 			&choosed[6]);
			ImGui::Checkbox("Trace", 			&choosed[7]);
			ImGui::Checkbox("Point light", 		&choosed[8]);

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
			if (auto* object = focusedObject->getComponentAs<PointLightComponent>()) object->renderHeader(_scenes, focusedObject);

		}

	}

	void LevelEditor::_renderGroupProperty() {
		if (ImGui::Button("Undo##group"))
		{
			_groupOrigin = glm::vec3();
			for (auto& tuple : _selectedEntitesBackup)
			{
				auto entity = std::get<0>(tuple);

				entity->setPosition(std::get<1>(tuple));
				entity->setRotation(std::get<2>(tuple));
				_groupOrigin += entity->getPosition();
			}
			_groupOrigin /= _selectedEntitesBackup.size();
		}

		ImGui::Text("Selected: %d", _objectsList.getSelectedEntities().size());

		ImGui::SameLine();
		if (ImGui::SmallButton("Unselect all##group")) {
			_objectsList.clearSelected();
		}

		if (ImGui::DragFloat3("Offset##group", &_currentGroupPosition[0])) {
			auto delta = _currentGroupPosition - _lastGroupPosition;

			_groupOrigin = glm::vec3();
			auto& group = _objectsList.getSelectedEntities();

			for (auto entity : group) {
				entity->move(delta);
				_groupOrigin += entity->getPosition();
			}

			_groupOrigin /= group.size();

			_lastGroupPosition = _currentGroupPosition;
		}

		if (ImGui::DragFloat3("Rotation##group", &_currentGroupRotation[0]))
		{
			for (auto entity : _objectsList.getSelectedEntities())
			{
				auto rotation = entity->getRotation() + (_currentGroupRotation - _lastGroupPosition);
				entity->setRotation(rotation);

				auto rotOffset = glm::radians(_currentGroupRotation - _lastGroupPosition);

				auto offset = entity->getPosition() - _groupOrigin;

				offset = glm::rotateX(offset, rotOffset.x);
				offset = glm::rotateY(offset, rotOffset.y);
				offset = glm::rotateZ(offset, rotOffset.z);

				entity->setPosition(_groupOrigin + offset);
			}

			_lastGroupPosition = _currentGroupRotation;
		}

		if (ImGui::TreeNodeEx("Selected entities##group", ImGuiTreeNodeFlags_DefaultOpen)) {
			for (auto entity : _objectsList.getSelectedEntities())
			{
				ImGui::Bullet();

				ImGui::SameLine();

				ImGui::Text("%s", entity->getName().c_str());
				
				ImGui::SameLine();

				ImGui::PushID(entity);
				if (ImGui::SmallButton("Unselect##group"))
				{
					_objectsList.removedSelected(entity);
					_updateGroup();
					ImGui::PopID();
					break;
				}
				ImGui::PopID();
			}

			ImGui::TreePop();
		}
	}
}
