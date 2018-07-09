#include "ObjectsList.hpp"

#include <imgui.h>

#include "../ScenesManager.hpp"
#include "../Entity.hpp"
#include "../Scene.hpp"

namespace rat {
	ObjectsList::ObjectsList(ScenesManager& scenes) :
	_scenes{scenes} {
		_tab = "single";
	}

	int ObjectsList::getSelectedID() {
		return _selectedEntityID;
	}

    Entity* ObjectsList::getSelectedEntity() {
    	return _scenes.getCurrentScene()->getEntity(_selectedEntityID);
    }

	bool ObjectsList::isAnySingleEntitySelected() {
		return _selectedEntityID != -1;
	}

	bool ObjectsList::isAnyEntitySelected()
	{
		return isAnySingleEntitySelected() || isGroupSelected();
	}

	bool ObjectsList::isEntitySelected(Entity* entity)
	{
		if (_selectedEntityID == entity->getID())
			return true;

		if (std::find(_selectedEntities.begin(), _selectedEntities.end(), entity) != _selectedEntities.end())
			return true;

		return false;
	}

	void ObjectsList::unselect() {
		_selectedEntityID = -1;

		_selectedEntities.clear();
	}

	void ObjectsList::render(bool& ifRender) {

		if(ImGui::Begin("Objects", &ifRender)) {
			ImGui::Separator();

			// Current scene
			auto* scene = _scenes.getCurrentScene();

			// Group buttons            
			int id = 0;
			float availWidth = (ImGui::GetContentRegionAvailWidth()-ImGui::GetStyle().ItemSpacing.x)*0.5f;
			for(auto& group : scene->getAllEntities()) {

				// Colorize button with selected group
				bool colorize = _tab == group.first;
				if(colorize) {
					ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Header]);
					ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_SeparatorHovered]);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);
				}
				if(ImGui::Button(group.first.c_str(), ImVec2(availWidth, 0))) {
					_tab = group.first;
				}
				if(colorize) {
					ImGui::PopStyleColor(3);
				}
				if(id%2 == 0) ImGui::SameLine();
				++id;
			}
			ImGui::Separator();

			// Operations buttons
			availWidth = (ImGui::GetContentRegionAvailWidth()-ImGui::GetStyle().ItemSpacing.x-ImGui::GetStyle().WindowPadding.x)*0.33333f;

			if(ImGui::Button("+##operation", ImVec2(availWidth, 0))) {
				addObjectToCurrentGroup();
			}
			if(isAnySingleEntitySelected() && getSelectedEntity()->getGroup() == _tab) {
				ImGui::SameLine();
				if(ImGui::Button("Clone##operation", ImVec2(availWidth, 0))) {
					duplicateObject(_selectedEntityID);
				}
				ImGui::SameLine();
				if(ImGui::Button("-##operation", ImVec2(availWidth, 0))) {
					removeObject(_selectedEntityID);
				}
			}
			ImGui::Separator();

			// Child for scroll and separate top part
			ImGui::BeginChild("Objects##list", ImVec2(ImGui::GetContentRegionAvailWidth(), 0), false, ImGuiWindowFlags_HorizontalScrollbar);
			// List of objects
			id = 0;
			auto& entities = scene->getEntities(_tab);
			for(auto& object : entities) {
				ImGui::PushID(id);
				if(ImGui::Selectable(object->getName().c_str(), _selectedEntityID == object->getID())) {
					if(_selectedEntityID != object->getID()) {
						_selectedEntityID = object->getID();
						clearSelected();
					}
					else {
						_selectedEntityID = -1;
					}
				}	

				// Swapping objects in list
				static int draggedObject = -1;
				if(ImGui::IsMouseReleased(0)) draggedObject = -1;
				if(ImGui::IsItemClicked()) draggedObject = id;
				if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && draggedObject != -1 && draggedObject != id) {
					std::swap(entities[id], entities[draggedObject]);
					draggedObject = id;
				}

				// Open context menu for object
				if(_renderObjectPopup(object.get())) {
					ImGui::PopID();
					break;
				}

				ImGui::PopID();
				++id;				
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}

	bool ObjectsList::_renderObjectPopup(Entity* entity) {
		if(ImGui::BeginPopupContextItem("##object_popup")) {
			if(ImGui::Selectable("Clone##object_popup")) {
				duplicateObject(entity->getID());
			}
			if(ImGui::Selectable("Remove##object_popup")) {
				removeObject(entity->getID());
				ImGui::EndPopup();
				return true;
			}

			if (entity->getGroup() == "path" || entity->getGroup() == "background" || entity->getGroup() == "foreground")
			{
				if (ImGui::BeginMenu("Move to##object_popup"))
				{
					if (entity->getGroup() != "path" && ImGui::Selectable("Path"))
					{
						auto scene = _scenes.getCurrentScene();
						scene->changeEntityGroup(entity, "path");
					}
					if (entity->getGroup() != "background" && ImGui::Selectable("Background"))
					{
						auto scene = _scenes.getCurrentScene();
						scene->changeEntityGroup(entity, "background");
					}
					if (entity->getGroup() != "foreground" && ImGui::Selectable("Foreground"))
					{
						auto scene = _scenes.getCurrentScene();
						scene->changeEntityGroup(entity, "foreground");
					}

					ImGui::EndMenu();
				}
			}

			ImGui::EndPopup();
		}
		return false;
	}

	void ObjectsList::update() {

	}

	void ObjectsList::select(Entity* object) {
		clearSelected();

		_selectedEntityID = object->getID();


	}

	void ObjectsList::select(int id) {
		clearSelected();
		_selectedEntityID = id;
	}

	void ObjectsList::addSelected(Entity* entity) {
		if (isAnySingleEntitySelected()) {
			_selectedEntities.push_back(getSelectedEntity());
			_selectedEntityID = -1;
		}

		if (auto it = std::find(_selectedEntities.begin(), _selectedEntities.end(), entity); it == _selectedEntities.end()) {
			_selectedEntities.push_back(entity);
		}
	}

	void ObjectsList::removedSelected(Entity* entity)
	{
		auto it = std::find(_selectedEntities.begin(), _selectedEntities.end(), entity);

		if (it != _selectedEntities.end())
		{
			_selectedEntities.erase(it);
		}

		//std::remove(_selectedEntities.begin(), _selectedEntities.end(), entity);
	}

	void ObjectsList::clearSelected()
	{
		_selectedEntities.clear();
	}

	const std::vector<Entity*>& ObjectsList::getSelectedEntities()
	{
		return _selectedEntities;
	}

	bool ObjectsList::isGroupSelected()
	{
		return !_selectedEntities.empty();
	}

	void ObjectsList::addObject(const std::string& groupName) {
		Entity* entity = _scenes.getCurrentScene()->addEntity(groupName);
		if(entity) {
			select(entity);
		}
	}

	void ObjectsList::addObjectToCurrentGroup() {
		addObject(_tab);
	}

	void ObjectsList::duplicateObject(int id) {
		Entity* entity = _scenes.getCurrentScene()->duplicateEntity(id);
		if(entity) select(entity);
	}

	void ObjectsList::removeObject(int id) {
		if(getSelectedID() == id) unselect();
		_scenes.getCurrentScene()->removeEntity(id);		
	}
}
