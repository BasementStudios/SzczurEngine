#include "MiniMap.hpp"

#include <tuple>

namespace rat {

	MiniMap::MiniMap(Script& script, Window& window, Input& input, bool& pause) :
		script(script), window(window), input(input), pause(pause) {
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
	
	MiniObjectPlayer* MiniMap::getPlayer() {		
		return player.get();
	}
	
	MiniObjectScene* MiniMap::getScene(const std::string& name) {
		for(auto& obj : scene) {
			if(obj->getName() == name) return obj.get();
		}
		return nullptr;
	}
	
	MiniObjectBackground* MiniMap::getBackground(const std::string& name) {
		for(auto& obj : background) {
			if(obj->getName() == name) return obj.get();
		}
		return nullptr;
	}
	
	sol::object MiniMap::lua_getSHD(MiniObject* object) {
		if(object == nullptr) return sol::make_object(script.get().lua_state(), sol::nil);
		if(object->type == MiniObject::Player) {
			return sol::make_object(script.get().lua_state(), player.get());
		}
		else if(object->type == MiniObject::Scene) {
			return sol::make_object(script.get().lua_state(), 
				dynamic_cast<MiniObjectScene*>(object));
		}
		else if(object->type == MiniObject::Background) {
			return sol::make_object(script.get().lua_state(), 
				dynamic_cast<MiniObjectBackground*>(object));
		}
		return sol::make_object(script.get().lua_state(), sol::nil);
	}
	sol::object MiniMap::lua_getSelected() {
		return lua_getSHD(selectedObject);
	}
	sol::object MiniMap::lua_getHovered() {
		return lua_getSHD(hoveredObject);		
	}
	sol::object MiniMap::lua_getDragged() {
		return lua_getSHD(draggedObject);		
	}	
	
	void MiniMap::update(float deltaTime) {
		time += deltaTime;
		player->update(deltaTime);
		bool waitForAction = true;
		for(auto& obj : scene) {
			obj->update(deltaTime);
			// sf::Vector2f tempSize(player->colliderSize.x/2.f, player->colliderSize.y/2.f);
			if(input.getManager().isPressed(Keyboard::Space)) {
				if(waitForAction && obj->isCollision(player->pos - player->colliderSize/2.f, player->colliderSize, player->colliderPos)) {
					if(obj->funcOnAction.valid()) {
						obj->action();
						waitForAction = false;
					}
				}
				// ImGui::Begin("Collision!", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
				// ImGui::Text("Yup! That's collision.");
				// ImGui::End();
			}
		}
	}
	
	void MiniMap::render(sf::RenderTexture &canvas) {
		if(pause) {
			for(auto& obj : background) {
				obj->render(canvas);
			}
		}
		else {
			for(auto& obj : background) {
				obj->render(canvas, player->pos);
			}		
		}
		for(auto& obj : scene) {
			obj->render(canvas);
		}		
		player->render(canvas);
	}

/////////////////////////////////////////// SCRIPT ///////////////////////////////////////////
	
	void MiniMap::runScript(const std::string& code) {	
		auto& lua = script.get();
		lua.set("THIS", this);
		try {
			lua.script(code);
		}
		catch(sol::error e) {
			std::cout<<"[ERROR] Cannot run script \n"<<std::flush;
			std::cout<<e.what()<<'\n'<<std::flush;
		}
	}
	
	void MiniMap::runFileScript(const std::string& filepath) {	
		auto& lua = script.get();
		lua.set("THIS", this);
		try {
			lua.script_file(filepath);
		}
		catch(sol::error e) {
			std::cout<<"[ERROR] Cannot load "<<filepath<<'\n'<<std::flush;
			std::cout<<e.what()<<'\n'<<std::flush;
		}
	}
	
	void MiniMap::initScript(Script& script) {
		auto object = script.newClass<MiniMap>("MiniMap", "MiniWorld");
		object.set("getSelectedObject", &MiniMap::lua_getSelected);
		object.set("getHoveredObject", &MiniMap::lua_getHovered);
		object.set("getDraggedObject", &MiniMap::lua_getDragged);
		object.set("newBackground", &MiniMap::newBackground);
		object.set("newScene", &MiniMap::newScene);
		object.set("getPlayer", &MiniMap::getPlayer);
		object.set("getScene", &MiniMap::getScene);
		object.set("getBackground", &MiniMap::getBackground);
		object.init();
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