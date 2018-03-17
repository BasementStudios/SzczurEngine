#include "MiniMap.hpp"

namespace rat {

	MiniMap::MiniMap(Script& script) :
		script(script) {
		
// TEMP
		objects.emplace_back(new MiniObject(script));
		objects.back()->addComponent("Base");
		objects.back()->addComponent("Color");
		objects.back()->runScript("THIS.Base.size.x = 400");
		objects.back()->runScript("THIS.Color.color = 2");
	}
	
	MiniBackgroundObject* MiniMap::newBackgroundObject() {
		backgroundObjects.emplace_back(new MiniBackgroundObject);
		backgroundObjects.back()->prepare(script);
		backgroundObjects.back()->loadScript("scripts/obj_1.lua");
		return backgroundObjects.back().get();
	}	
	
	MiniPathObject* MiniMap::newPathObject() {		
		pathObjects.emplace_back(new MiniPathObject);
		pathObjects.back()->prepare(script);
		pathObjects.back()->loadScript("scripts/obj_1.lua");
		return pathObjects.back().get();
	}
	
	void MiniMap::render(sf::RenderTexture &canvas) {
#ifdef EDITOR	
		static auto drawObjectFrame = [&](auto object) {				
			// auto obj = dynamic_cast<EditorObject*>(object);
			if(object == draggedObject) object->editor_ObjectFrame(canvas, 3);
			else if(object == selectedObject) object->editor_ObjectFrame(canvas, 2);
			else if(object == hoveredObject) object->editor_ObjectFrame(canvas, 1);
			else object->editor_ObjectFrame(canvas, 0);	
		};
#endif		
		for(auto& obj : backgroundObjects) {
			obj->render(canvas);
#ifdef EDITOR
			if(objectType == 0) drawObjectFrame(obj.get());
#endif
		}
		for(auto& obj : pathObjects) {
			obj->render(canvas);	
#ifdef EDITOR
			if(objectType == 1) drawObjectFrame(obj.get());
#endif
		}
		
		for(auto& obj : objects) {
			obj->render(canvas);
		}		
	}
	
	void MiniMap::update() {		
		for(auto& obj : pathObjects) {
			obj->update();
		}
		
		for(auto& obj : objects) {
			obj->update();
		}	
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
		
		if(ImGui::Combo("##Object types", &objectType, "Background\0Scene\0\0")) {
			selectedObjectId = -1;
		}		
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
				addingObject = false;
				std::string newName(nameObjectBuffer);
				if(newName != "") {
					if(addingObject) {
						if(objectType == 0) {
							newBackgroundObject()->setName(newName);
						}
						else if(objectType == 1) {
							newPathObject()->setName(newName);
						}
					}
					else if(renamingObject) {						
						if(objectType == 0) {
							selectedObject->setName(newName);
						}
						else if(objectType == 1) {
							selectedObject->setName(newName);
						}
					}
				}
				*nameObjectBuffer = '\0';
			}
		}
		
		// List of objects 
		ImGui::Text("List of objects:");
		static auto drawList = [&](auto& objects) {
			for (int i=0; i<objects.size(); ++i) {
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
		};
		
		
		if(objectType == 0) drawList(backgroundObjects);	
		else if(objectType == 1) drawList(pathObjects);
	}
	
//////////////////// Object editor ////////////////////

	void MiniMap::editorObjectEditor() {
		
		static auto drawObjectEditor = [&](auto object) {
			objectEditorIsOpen = true;
			ImGui::Begin("Object editor", &objectEditorIsOpen);			
			if(!objectEditorIsOpen) {
				selectedObjectId = -1;
			}
			ImGui::Text("Object: %s", object->getName().c_str());
			object->editor(); // <--- There is everything about editing object
			ImGui::End();		
		};
		
		if(selectedObjectId!=-1) {
			if(objectType == 0) drawObjectEditor(backgroundObjects[selectedObjectId].get());
			else if(objectType == 1) drawObjectEditor(pathObjects[selectedObjectId].get());
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
		
		if(selectedObjectId != -1) {
			if(objectType == 0) selectedObject = backgroundObjects[selectedObjectId].get();
			else if(objectType == 1) selectedObject = pathObjects[selectedObjectId].get();
		}
		
		static auto dragObject = [&](auto& objects) {
			int i = -1;
			for(auto& obj : objects) {
				++i;
				
				if(checkCollision(obj->pos, obj->size, mouse)) {
					hoveredObject = obj.get();
					if(!ImGui::IsMouseHoveringAnyWindow() && input.getManager().isPressed(Mouse::Left)) {
						draggedObject = obj.get();
						deltaClick.x = draggedObject->getPosition().x - mouse.x;
						deltaClick.y = draggedObject->getPosition().y - mouse.y;
						selectedObjectId = i;
						break;
					}
					break;
				}
			}	
		};
		
		if(!draggedObject) {
			if(objectType == 0) dragObject(backgroundObjects);
			else if(objectType == 1) dragObject(pathObjects);
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
	}
#endif
}