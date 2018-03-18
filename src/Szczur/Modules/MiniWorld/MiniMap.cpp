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
	
	MiniObjectScene* MiniMap::newScene() {
		scene.emplace_back(new MiniObjectScene(script));
		return scene.back().get();
	}
	
	MiniObjectBackground* MiniMap::newBackground() {
		background.emplace_back(new MiniObjectBackground(script));
		return background.back().get();
	}	
	
	// MiniObject* MiniMap::newObject() {	
		// objects.emplace_back(new MiniObject(script));
		// return objects.back().get();
	// }
	
	void MiniMap::update(float deltaTime) {
		time += deltaTime;
		player->update(deltaTime);
		for(auto& obj : scene) {
			obj->update(deltaTime);
			// sf::Vector2f tempSize(player->colliderSize.x/2.f, player->colliderSize.y/2.f);
			if(obj->isCollision(player->pos - player->colliderSize/2.f, player->colliderSize, player->colliderPos)) {
				ImGui::Begin("Collision!", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
				ImGui::Text("Yup! That's collision.");
				ImGui::End();
			}
		}
	}
	
	void MiniMap::render(sf::RenderTexture &canvas) {
		for(auto& obj : background) {
			obj->render(canvas);
		}
		for(auto& obj : scene) {
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
		for(auto& obj : background) {
			drawFrame(obj.get());
		}
		for(auto& obj : scene) {
			drawFrame(obj.get());
			obj->renderCollider(canvas);
		}
		drawFrame(player.get());
		player->renderCollider(canvas);
	}	

	void MiniMap::editor() {		
	
		ImGui::Begin("Map editor", nullptr);
		
		// Adding objects
		ImGui::Text("Add new object:");
		if(ImGui::Button("Scene##Button", {80, 0})) {
			newScene();
		}
		ImGui::SameLine();
		if(ImGui::Button("Background##Button", {80, 0})) {
			newBackground();
		}		

//////////////////////////////// LIST OF OBJECTS ////////////////////////////////
		
		
// ================== Function for tree ==================
		static auto makeTreeSection = [&](auto object, int gr, int el) {
			if(!object->getFrame().getClickable()) ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(0.4f,0.2f,0.2f,1.0f));
			if(ImGui::Selectable(object->getName().c_str(), group==gr && element==el)) {
				selectedObject = object;
				group = gr;
				element = el;
			}
			if(!object->getFrame().getClickable()) ImGui::PopStyleColor(1);			
			if(ImGui::BeginPopupContextItem())
			{				
				if(ImGui::Selectable(object->getFrame().getClickable() ? "Lock" : "Unlock")) {
					object->getFrame().setClickable(!object->getFrame().getClickable());
				}
				ImGui::EndPopup();
			}
		};
		
// ================== Single ==================

		if(ImGui::TreeNode("Single")) {
			makeTreeSection(player.get(), 0, 0);
			
			ImGui::TreePop();
		}
// ================== Scene ==================
		if(ImGui::TreeNode("Scene##Tree")) {
			for(unsigned int i = 0; i<scene.size(); ++i) {
				ImGui::PushID(i);
				makeTreeSection(scene[i].get(), 1, i);
				ImGui::PopID();
			}
			ImGui::TreePop();
		}		
// ================== Background ==================
		if(ImGui::TreeNode("Background##Tree")) {
			for(unsigned int i = 0; i<background.size(); ++i) {
				ImGui::PushID(i+scene.size());
				makeTreeSection(background[i].get(), 2, i);
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
		for(int i = scene.size()-1; i>=0; --i) {			
			auto obj = scene[i].get();
			if(obj->getFrame().isHovered(point)) {
				hoveredObject = obj;
				return;
			}
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
				
				if(selectedObject == player.get()) {group = 0; element = 0;}
				else if(dynamic_cast<MiniObjectScene*>(selectedObject)) {
					for(unsigned int i = 0; i<scene.size(); ++i) {
						if(selectedObject == scene[i].get()) {
							group = 1; element = i;
						}
					}
				}
				else if(dynamic_cast<MiniObjectBackground*>(selectedObject)) {
					for(unsigned int i = 0; i<background.size(); ++i) {
						if(selectedObject == background[i].get()) {
							group = 2; element = i;
						}
					}
				}				
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