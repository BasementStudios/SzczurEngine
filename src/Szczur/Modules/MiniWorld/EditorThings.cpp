#include "Szczur/Modules/Script/Script.hpp"

#include "MiniWorld.hpp"
#include "MiniMap.hpp"
#include "MiniBackgroundObject.hpp"
#include "MiniPathObject.hpp"


/*
	Plan of this file:
	/////////////////////////////////////////// EDITOR ///////////////////////////////////////////
	
	// ==================== INITIALIZING MEMBERS FOR SCRIPT ====================
	// -------- Background object --------
	
	// -------- Path object --------
	
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
	
	// ==================== OBJECT EDITOR ====================
	// -------- Properties panel for background object --------
	For example:
	a) Base (with pos and size)
	b) Visual (with sprite/armature)
	c) Script (with .lua file)

	// -------- Properties panel for path object --------
	For example:
	a) Base (with pos and size)
	b) Visual (with sprite/armature)
	c) Script (with .lua file)
	
	// ==================== OUTSIDE ====================
	For example: Console
*/


namespace rat {

/////////////////////////////////////////// EDITOR ///////////////////////////////////////////

// ==================== INITIALIZING MEMBERS FOR SCRIPT ====================
// -------- Background object --------
	void MiniBackgroundObject::ET_initializeScript(ScriptClass<rat::MiniBackgroundObject>& object) {
		/*
			At first you need to add new member in special area for EDITOR THINGS (in MiniBackgroundObject.hpp)
		*/
		
		// e.g. object.set("color", &MiniBackgroundObject::color);
	}
	
// -------- Path object --------
	void MiniPathObject::ET_initializeScript(ScriptClass<rat::MiniPathObject>& object) {
		/*
			At first you need to add new member in special area for EDITOR THINGS (in MiniPathObject.hpp)
		*/
		
		// e.g. object.set("color", &MiniPathObject::color);
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
	void MiniMap::ET_popupOnList(EditorObject* object) {
		
		/*
			When `objectType` == 0 => dynamic_cast<MiniBackgroundObject*>(object)
			When `objectType` == 1 => dynamic_cast<MiniPathObject*>(object)
		*/
		
		/*
			e.g.
			if(ImGui::Selectable("Dance!")) {
				
			}
		*/
	}

// ==================== OBJECT EDITOR ====================
// -------- Properties panel for background object --------
	void MiniBackgroundObject::ET_propertiesPanel() {
		ImGui::Separator();
		ImGui::Text("I'm new properties group :D");	
		/*
			e.g.			
			ImGui::DragFloat("##SizeX", &colliderSize.x); //<--- `##` makes name invisible in gui
			ImGui::SameLine();
			ImGui::DragFloat("##SizeY", &colliderSize.y);
			ImGui::SameLine();	
		*/
	}
	
// -------- Properties panel for path object --------
	void MiniPathObject::ET_propertiesPanel() {		
		ImGui::Separator();
		ImGui::Text("I'm new properties group :D");
		/*
			e.g.			
			ImGui::DragFloat("##ColliderSizeX", &colliderSize.x); //<--- `##` makes name invisible in gui
			ImGui::SameLine();
			ImGui::DragFloat("##ColliderSizeY", &colliderSize.y);
			ImGui::SameLine();	
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