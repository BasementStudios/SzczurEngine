#include "MiniMap.hpp"

#include <tuple>

namespace rat {

	MiniMap::MiniMap(Script& script, Window& window, Input& input) :
		script(script), window(window), input(input) {
		init();
	}
	
	void MiniMap::init() {
		player.reset(new MiniObjectPlayer(script, input));
	}
	
	MiniObjectBackground* MiniMap::newBackground() {
		background.emplace_back(new MiniObjectBackground(script));
		return background.back().get();
	}
	
	// MiniObject* MiniMap::newObject() {	
		// objects.emplace_back(new MiniObject(script));
		// return objects.back().get();
	// }
	
	void MiniMap::update() {
		player->update();
	}
	
	void MiniMap::render(sf::RenderTexture &canvas) {
		for(auto& obj : background) {
			obj->render(canvas);
		}
		
		player->render(canvas);
	}

/////////////////////////////////////////// EDITOR ///////////////////////////////////////////

#ifdef EDITOR

	void MiniMap::editorRender(sf::RenderTexture& canvas) {	
		static auto drawFrame = [&](auto object) {
			if(object == draggedObject) canvas.draw(object->getFrame().getShape(3));
			else if(object == selectedObject) canvas.draw(object->getFrame().getShape(2));
			else if(object == hoveredObject) canvas.draw(object->getFrame().getShape(1));
			else canvas.draw(object->getFrame().getShape(0));
		};
		drawFrame(player.get());
		for(auto& obj : background) {
			drawFrame(obj.get());
		}
	}	

	void MiniMap::editor() {		
	
		ImGui::Begin("Map editor", nullptr);
		
		// Adding objects
		ImGui::Text("Add new object");
		if(ImGui::Button("Background##Button", {80, 0})) {
			newBackground();
		}
		

//////////////////////////////// LIST OF OBJECTS ////////////////////////////////
		
// ================== Single ==================

		if(ImGui::TreeNode("Single")) {
			if(!player->getFrame().getClickable()) ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(0.4f,0.2f,0.2f,1.0f));
			if(ImGui::Selectable(player->getName().c_str(), group==0 && element==0)) {
				selectedObject = player.get();
				group = 0;
				element = 0;
			}			
			if(!player->getFrame().getClickable()) ImGui::PopStyleColor(1);			
			
			if(ImGui::BeginPopupContextItem())
			{				
				if(ImGui::Selectable(player->getFrame().getClickable() ? "Lock" : "Unlock")) {
					player->getFrame().setClickable(!player->getFrame().getClickable());
				}
				ImGui::EndPopup();
			}
			
			ImGui::TreePop();
		}
// ================== Background ==================
		if(ImGui::TreeNode("Background##Tree")) {
			for(unsigned int i = 0; i<background.size(); ++i) {
				auto object = background[i].get();
				ImGui::PushID(i);
				if(!object->getFrame().getClickable()) ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(0.4f,0.2f,0.2f,1.0f));
				if(ImGui::Selectable(object->getName().c_str(), group==1 && element==i)) {
					selectedObject = object;
					group = 1;
					element = i;
				}
				if(!object->getFrame().getClickable()) ImGui::PopStyleColor(1);
				
				if(ImGui::BeginPopupContextItem())
				{				
					if(ImGui::Selectable(object->getFrame().getClickable() ? "Lock" : "Unlock")) {
						object->getFrame().setClickable(!object->getFrame().getClickable());
					}
					ImGui::EndPopup();
				}
				
				ImGui::PopID();
			}
			ImGui::TreePop();
		}		
		ImGui::End();
		
		if(!ImGui::IsAnyWindowHovered()) {
			editorDragAndDrop();
		}
		
		static bool editorIsOpen = false;
		if(selectedObject) {		
			editorIsOpen = true;
			ImGui::Begin("Object editor", &editorIsOpen);			
			if(editorIsOpen) {
				selectedObject->editor();
				editorIsOpen = false;
			}
			else {
				selectedObject = nullptr;
				element = -1;
				group = -1;
			}
			ImGui::End();			
		}
	}
		
	void MiniMap::editorHoveredUpdate(const sf::Vector2f& point) {
		hoveredObject = nullptr;
		if(player->getFrame().isHovered(point)) {
			hoveredObject = player.get();
			return;
		}
		for(int i = background.size()-1; i>=0; --i) {			
			auto obj = background[i].get();
			if(obj->getFrame().isHovered(point)) {
				hoveredObject = obj;
				return;
			}
		}
	}
	
	void MiniMap::editorDragAndDrop() {
		
		sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(window.getWindow()));
		static sf::Vector2f deltaClick(0,0);
		
		editorHoveredUpdate(mouse);		
		
		if(hoveredObject) {
			if(input.getManager().isPressed(Mouse::Left)) {
				selectedObject = hoveredObject;
				draggedObject = hoveredObject;
				deltaClick = *hoveredObject->getFrame().pos - mouse;
			}
		}
		
		if(draggedObject) {
			if(input.getManager().isKept(Mouse::Left)) {
				selectedObject->setPosition(mouse + deltaClick);
			}
			if(input.getManager().isUnkept(Mouse::Left)) {
				draggedObject = nullptr;
			}
		}
	}
	
	
#endif
}