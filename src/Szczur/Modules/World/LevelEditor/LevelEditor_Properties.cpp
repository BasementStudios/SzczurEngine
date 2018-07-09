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
#include "../Components/AudioComponent.hpp"

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
				choosed[9] = entity->hasComponent<AudioComponent>();
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
				UPDATE_COMPONENT(AudioComponent, 			9);

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

			if (entity->getGroup() == "path" || entity->getName() == "Player")
				ImGui::Checkbox("Collider", 	&choosed[3]);

			ImGui::Checkbox("Scriptable", 		&choosed[4]);
			ImGui::Checkbox("Interactable", 	&choosed[5]);
			ImGui::Checkbox("Trigger", 			&choosed[6]);
			ImGui::Checkbox("Trace", 			&choosed[7]);
			ImGui::Checkbox("Point light", 		&choosed[8]);
			ImGui::Checkbox("Audio", 			&choosed[9]);

			ImGui::EndPopup();
		}
	}

#define COPY_COMPONENT(COMP, NAME) if(focusedObject->hasComponent< COMP >()) { if (ImGui::Button(NAME)) { _componentToCopy = focusedObject->getComponent<COMP>(); ImGui::CloseCurrentPopup(); } }

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

		_renderComponentsManager();

		ImGui::SameLine();

		// copy component
		ImGui::Button("Copy");
		if (ImGui::BeginPopupContextItem("Copy##context", 0)) {
			COPY_COMPONENT(SpriteComponent,			"SpriteComponent");
			COPY_COMPONENT(AnimatedSpriteComponent,	"AnimatedSpriteComponent");
			COPY_COMPONENT(ArmatureComponent,		"ArmatureComponent");
			COPY_COMPONENT(ColliderComponent,		"ColliderComponent");
			COPY_COMPONENT(ScriptableComponent,		"ScriptableComponent");
			COPY_COMPONENT(InteractableComponent,	"InteractableComponent");
			COPY_COMPONENT(TriggerComponent,		"TriggerComponent");
			COPY_COMPONENT(TraceComponent,			"TraceComponent");
			COPY_COMPONENT(PointLightComponent,		"PointLightComponent");
			COPY_COMPONENT(AudioComponent,			"AudioComponent");
			ImGui::EndPopup();
		}
		ImGui::SameLine();

		// paste component
		ImGui::Button("Paste");
		if (ImGui::BeginPopupContextItem("Paste##context", 0)) {
			if (_componentToCopy) {
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 200);
				ImGui::Text("Are you sure you want to paste");

				ImGui::TextColored(ImVec4(1.f, 0.3f, 0.3f, 1.f), _componentToCopy->getName().c_str());
				ImGui::SameLine();
				ImGui::Text("from");
				ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), _componentToCopy->getEntity()->getName().c_str());
				ImGui::SameLine();
				ImGui::Text("?");

				if (ImGui::Button("Yes##paste")) {
					if (focusedObject->hasComponent(_componentToCopy->getComponentID())) {
						focusedObject->removeComponent(_componentToCopy->getComponentID());
					}

					focusedObject->addComponent(_componentToCopy->copy(focusedObject));

					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("No##paste")) {
					ImGui::CloseCurrentPopup();
				}
			}
			else {
				ImGui::Text("Nothing to paste");
			}

			ImGui::EndPopup();
		}

		if (_scenes.isGameRunning()) {
			ImGui::SameLine();
			if (ImGui::Button("Update status##properties")) {
				_scenes.saveEntityToConfig(focusedObject, _scenes.getRunConfig());
				printMenuBarInfo(std::string("Updated status for: ") + focusedObject->getName());
			}
		}

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
			if (auto* object = focusedObject->getComponentAs<AudioComponent>()) object->renderHeader(_scenes, focusedObject);

		}
	}

	void LevelEditor::_resetGroupProperties()
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

	void LevelEditor::_updateGroupProperites(const glm::vec3& pos, const glm::vec3& rot)
	{
		_groupOrigin = glm::vec3();

		for (auto& tuple : _selectedEntitesBackup)
		{
			auto& entity = std::get<0>(tuple);
			entity->setPosition(std::get<1>(tuple) + _currentGroupPosition);
			_groupOrigin += entity->getPosition();
		}

		_groupOrigin /= _selectedEntitesBackup.size();

		for (auto& tuple : _selectedEntitesBackup)
		{
			auto& entity = std::get<0>(tuple);

			auto rotation = std::get<2>(tuple) + _currentGroupRotation;
			entity->setRotation(rotation);

			auto rotOffset = glm::radians(_currentGroupRotation);

			auto offset = entity->getPosition() - _groupOrigin;

			offset = glm::rotateX(offset, rotOffset.x);
			offset = glm::rotateY(offset, rotOffset.y);
			offset = glm::rotateZ(offset, rotOffset.z);

			entity->setPosition(_groupOrigin + offset);
		}
	}

	void LevelEditor::_renderGroupProperty() {
		if (ImGui::Button("Undo##group"))
		{
			_resetGroupProperties();

			_currentGroupPosition = glm::vec3();
			_currentGroupRotation = glm::vec3();
		}

		ImGui::Text("Selected: %d", _objectsList.getSelectedEntities().size());

		ImGui::SameLine();
		if (ImGui::SmallButton("Unselect all##group")) {
			_objectsList.clearSelected();
		}

		if (ImGui::DragFloat3("Offset##group", &_currentGroupPosition[0])) {
			_updateGroupProperites(_currentGroupPosition, _currentGroupRotation);
		}

		if (ImGui::DragFloat3("Rotation##group", &_currentGroupRotation[0]))
		{
			_updateGroupProperites(_currentGroupPosition, _currentGroupRotation);
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
