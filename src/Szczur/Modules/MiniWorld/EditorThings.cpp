#include "Szczur/Modules/Script/Script.hpp"

#include "MiniWorld.hpp"
#include "MiniMap.hpp"

// Components
#include "Components/ComponentBase.hpp"
#include "Components/ComponentColor.hpp"


/*
	Plan of this file:
	/////////////////////////////////////////// EDITOR ///////////////////////////////////////////

	// ==================== MAIN EDITOR ====================
	// -------- Menu bar --------
	For example: 
	a) File
	b) Tools
	
	// -------- Main panel --------
	For example: 
	a) Buttons to pause/resume game
	b) Change editing map
	c) Rename map
	
	// -------- Map panel (above the list)  --------
	For example: List with objects on map
	
	// -------- Popup after RMB on object in list --------
		
	// ==================== OUTSIDE ====================
	For example: Console
*/


namespace rat {

/////////////////////////////////////////// EDITOR ///////////////////////////////////////////

// ==================== COMPONENTS ====================
	std::vector<std::string> MiniObject::availableComponents;
	void MiniObject::initComponents(Script &script) {
		//@todo Initializer list
		availableComponents.emplace_back("Base");
		availableComponents.emplace_back("Color");
		script.initClasses<
			ComponentColor, ComponentBase
		>();
	}
	
	void MiniObject::addComponent(const std::string& componentName) {
		auto result = std::find_if(components.begin(), components.end(), [&](auto& obj) { return obj->getComponentName()==componentName; });
		if(result == components.end()) {
			if(componentName == "Base")  addComponent<ComponentBase>();
			else if(componentName == "Color") addComponent<ComponentColor>();
		}
	}

// ==================== MAIN EDITOR ====================
// -------- Menu bar --------
	void MiniWorld::ET_menuBar() {
		/*
			`map` is a current map
		*/
		
		if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("Item", nullptr)) {
				std::cout<<"I'm from File/Item"<<std::endl;
			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Tools")) {
			ImGui::EndMenu();
		}
	}

// -------- Main panel --------
	void MiniWorld::ET_mainPanel() {
		/*
			`selectedObject` is a selected object
		*/
		
		// e.g. 
		ImGui::Text("Main panel");
	}
	
// -------- Map panel (above the list)  --------
	void MiniMap::ET_mapPanel() {
		/*
			`selectedObject` is a selected object
		*/
		
		// e.g. 
		ImGui::Text("Map panel");
	}

// -------- Popup after RMB on object in list --------
	void MiniMap::ET_popupOnList(MiniObject* object) {
		
		/*
			auto comp = object.findComponent("Color"); //return nullptr when component isn't inside
		*/
		
		/*
			e.g.
			if(ImGui::Selectable("Dance!")) {
				
			}
		*/
	}

// ==================== OUTSIDE ====================
	void MiniWorld::ET_outside() {
		
			//e.g.
			ImGui::Begin("Outside", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Text("Hello world!");
			ImGui::End();
		
	}


}