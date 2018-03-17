#include "MiniMap.hpp"

#include <tuple>

namespace rat {

	MiniMap::MiniMap(Script& script) :
		script(script) {
	}
	MiniObject* MiniMap::newObject() {	
		objects.emplace_back(new MiniObject(script));
		// objects.back()->addComponent("Base");
		// objects.back()->addComponent("Color");
		return objects.back().get();
	}
	
	void MiniMap::render(sf::RenderTexture &canvas) {
		for(auto& obj : objects) {
			obj->render(canvas);
#ifdef EDITOR
			auto object = obj.get();
			if(object == draggedObject) editorDrawObjectFrame(canvas, object, 3);
			else if(object == selectedObject) editorDrawObjectFrame(canvas, object, 2);
			else if(object == hoveredObject) editorDrawObjectFrame(canvas, object, 1);
			else editorDrawObjectFrame(canvas, object, 0);	
#endif
		}		
	}
	
	void MiniMap::update() {		
		for(auto& obj : objects) {
			obj->update();
		}
		// for(auto& obj : pathObjects) {
			// obj->update();
		// }
		
		// for(auto& obj : objects) {
			// obj->update();
		// }	
	}
	

/////////////////////////////////////////// EDITOR ///////////////////////////////////////////

#ifdef EDITOR

//////////////////// Objects list ////////////////////
	
	void MiniMap::editorObjectsList() {
		
		static char nameObjectBuffer[40];
		static bool addingObject = false;
		static bool renamingObject = false;
		
		// Combobox with object types		
		//if(ImGui::Combo("##Object types", &objectType, "Background\0Scene\0Battle\0Foreground\0\0")) {
		
		ET_mapPanel();
		
		// if(ImGui::Combo("##Object types", &objectType, "Background\0Scene\0\0")) {
			// selectedObjectId = -1;
		// }		
		
		ImGui::SameLine(ImGui::GetWindowWidth()-40);
		if(ImGui::Button(addingObject ? "-" : "+", {30, 0})) {
			if(addingObject == false) {
				addingObject = true;
				renamingObject = false;
				ImGui::SetKeyboardFocusHere(1);
			}
			else {
				addingObject = false;
				renamingObject = false;
			}			
		}
				
		// Create/rename object
		if(addingObject || renamingObject) {
			if(ImGui::InputText("Name", nameObjectBuffer, 40, ImGuiInputTextFlags_EnterReturnsTrue)) {
				std::string newName(nameObjectBuffer);
				if(newName != "") {
					if(addingObject) {
						newObject()->setName(newName);
					}
					else if(renamingObject) {	
						selectedObject->setName(newName);
					}
				}
				addingObject = false;
				renamingObject = false;
				*nameObjectBuffer = '\0';
			}
		}
		
		// List of objects 
		ImGui::Text("List of objects:");		
		for (int i = 0; i<objects.size(); ++i) {
			if(ImGui::Selectable(objects[i]->getName().c_str(), selectedObjectId == i)) {
				selectedObjectId = i;
			}
			if(ImGui::BeginPopupContextItem())
			{					
				if(ImGui::Selectable("Edit")) {
					selectedObjectId = i;
				}
				if(ImGui::Selectable("Rename")) {
					selectedObjectId = i;
					renamingObject = true;
				}
				if(ImGui::Selectable("Remove")) {
					selectedObjectId = -1;
					objects.erase(objects.begin() + selectedObjectId);
				}
				ET_popupOnList(objects[i].get());
				ImGui::EndPopup();
			}
		}
		if(selectedObjectId>=objects.size()) selectedObjectId = -1;
	}
	
//////////////////// Object editor ////////////////////

	void MiniMap::editorObjectEditor() {
		
		if(selectedObjectId != -1) {			
			auto object = objects[selectedObjectId].get();
			objectEditorIsOpen = true;
			ImGui::Begin("Object editor", &objectEditorIsOpen);			
			if(!objectEditorIsOpen) {
				selectedObjectId = -1;
			}			
			ImGui::Text("%s", object->getName().c_str());
			
			if(ImGui::Button("Edit components", {-1,0})) {
				ImGui::OpenPopup("Components");
			}
			
			object->editor();
			editorComponentsChanger();
			ImGui::End();
		}		
	}
	
//////////////////// Drag & drop ////////////////////

	void MiniMap::editorDragAndDrop(Window& window, Input& input) {
		
		static auto checkCollision = [](auto& pos, auto& size, auto& point) {
			return point.x>pos.x && point.y>pos.y && point.x<pos.x+size.x && point.y<pos.y+size.y;
		};
		
		static sf::Vector2i deltaClick{0,0};		
		auto mouse = sf::Mouse::getPosition(window.getWindow());		
			
		hoveredObject = nullptr;
		selectedObject = nullptr;
		
		for(unsigned int i = 0; i<objects.size(); ++i) {
			auto object = objects[i].get();
			
			if(checkCollision(object->getPosition(), object->getSize(), mouse)) {
				hoveredObject = object;
				if(!ImGui::IsMouseHoveringAnyWindow() && input.getManager().isPressed(Mouse::Left)) {
					draggedObject = object;
					deltaClick.x = draggedObject->getPosition().x - mouse.x;
					deltaClick.y = draggedObject->getPosition().y - mouse.y;
					selectedObjectId = i;
					break;
				}
				break;
			}
		}	
		
		if(hoveredObject) {
			ImGui::SetNextWindowPos(sf::Vector2i(mouse.x+1, mouse.y+1));
			ImGui::Begin("Tip", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysUseWindowPadding);
			// ImGui::Text("%d %d", mouse.x, mouse.y);
			ImGui::Text(hoveredObject->getName().c_str());
			ImGui::End();
		}
		
		if(draggedObject) {
			if(input.getManager().isKept(Mouse::Left)) {
				draggedObject->setPosition(sf::Vector2f(mouse + deltaClick));
			}		
			else if(input.getManager().isUnkept(Mouse::Left)) {
				draggedObject = nullptr;
			}		
		}
		
		if(selectedObjectId != -1) selectedObject = objects[selectedObjectId].get();
	}	
	
	void MiniMap::editorComponentsChanger() {	
	
		static bool firstTime = true;
		static std::vector<std::tuple<bool, std::string>> memComponents;
		
		if(firstTime) {
			firstTime = false;
			memComponents.clear();
			for(auto& comp : MiniObject::availableComponents) {
				Component* result = selectedObject->findComponent(comp);
				if(result) {
					memComponents.emplace_back(true, comp);
				}
				else {
					memComponents.emplace_back(false, comp);
				}
			}
		}
		
		if(ImGui::BeginPopupModal("Components", nullptr, ImGuiWindowFlags_NoResize|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings)) {
			ImGui::Text("List of components:");
			ImGui::BeginChild("##Components list", {200, 300});
			int i=-1; 
			for(auto& comp : memComponents) {				
				++i;				
                ImGui::PushID(i);
                if(std::get<0>(comp)) {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2/7.0f, 0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2/7.0f, 0.6f, 0.6f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2/7.0f, 0.8f, 0.8f));
					if(ImGui::Button("UNSET", {50, 0})) {
						std::get<0>(comp) = false;
					}
					ImGui::PopStyleColor(3);
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0/7.0f, 0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0/7.0f, 0.6f, 0.6f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0/7.0f, 0.8f, 0.8f));
					if(ImGui::Button("SET", {50, 0})) {
						std::get<0>(comp) = true;
					}
					ImGui::PopStyleColor(3);
				}
				ImGui::SameLine();
				ImGui::Selectable(std::get<1>(comp).c_str());	
                ImGui::PopID();
			}
			ImGui::EndChild();
			if(ImGui::Button("Accept", {80, 0})) {
				for(auto& comp : memComponents) {					
					if(std::get<0>(comp) == true) {
						selectedObject->addComponent(std::get<1>(comp));
					}
					else {
						selectedObject->removeComponent(std::get<1>(comp));						
					}
				}
				firstTime = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if(ImGui::Button("Close", {80, 0})) {
				firstTime = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	
	void MiniMap::editorDrawObjectFrame(sf::RenderTexture &canvas, MiniObject* object, int state) {  
		sf::RectangleShape shape; 
		shape.setSize({object->size.x+4, object->size.y+4}); 
		shape.setPosition({object->pos.x-2, object->pos.y-2}); 
		shape.setFillColor({0,0,0,0}); 
		if(state == 0) { 
			shape.setOutlineThickness(1); 
			shape.setOutlineColor({70,70,70,255}); 
		} 
		else if(state == 1) { 
			shape.setOutlineThickness(1); 
			shape.setOutlineColor({140,140,140,255}); 
		} 
		else if(state == 2) { 
			shape.setOutlineThickness(1); 
			shape.setOutlineColor({210,140,140,255}); 
		} 
		else if(state == 3) { 
			shape.setOutlineThickness(2); 
			shape.setOutlineColor({210,140,140,255}); 
		} 
		canvas.draw(shape); 
	} 
	
#endif
}