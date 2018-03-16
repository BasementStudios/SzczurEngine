#include "MiniBackgroundObject.hpp"

namespace rat {
	
	
	void MiniBackgroundObject::init() {
		if(funcInit.valid()) funcInit(this);
	}
	
	void MiniBackgroundObject::update() {
		if(funcUpdate.valid()) funcUpdate(this);
	}
	
	void MiniBackgroundObject::render(sf::RenderTexture &canvas) {
		sf::RectangleShape shape;
		shape.setSize(size);
		shape.setPosition(pos);
		shape.setOutlineThickness(-3);
		shape.setOutlineColor({0,0,0,255});
		if(color == 0) {
			shape.setFillColor({200,70,70,255});
		}
		else if(color == 1) {
			shape.setFillColor({70,70,200,255});
		}
		canvas.draw(shape);
	}	
	
	const std::string& MiniBackgroundObject::getName() {
		return name;
	}	
	
	void MiniBackgroundObject::setName(const std::string& _name) {
		name = _name;
	}
	
	const sf::Vector2f& MiniBackgroundObject::getPosition() {
		return pos;
	}	
	
	void MiniBackgroundObject::setPosition(const sf::Vector2f& _pos) {
		pos = _pos;
	}	
	
/////////////////////////////////////////// SCRIPT ///////////////////////////////////////////
	
	void MiniBackgroundObject::loadScript(const std::string& filepath) {
		base.loadScript<MiniBackgroundObject>(this, filepath);
	}
	
	void MiniBackgroundObject::runFileScript(const std::string& filepath) {
		base.runFileScript<MiniBackgroundObject>(this, filepath);
	}
	
	void MiniBackgroundObject::runScript(const std::string& code) {
		base.runScript<MiniBackgroundObject>(this, code);
	}
	
	ScriptableBase& MiniBackgroundObject::getBase() {
		return base;
	}
	
	void MiniBackgroundObject::prepare(Script& script) {
		auto& object = base.prepare(script);		
		object.set("this", this);
	}
	
	void MiniBackgroundObject::initScript(Script& script) {		
		auto object = script.newClass<MiniBackgroundObject>("MiniBackgroundObject", "MiniWorld");
		
		object.set("pos", &MiniBackgroundObject::pos);
		object.set("size", &MiniBackgroundObject::size);
		object.set("color", &MiniBackgroundObject::color);
		
		object.set("_init", &MiniBackgroundObject::funcInit);
		object.set("_update", &MiniBackgroundObject::funcUpdate);
		
		ET_initializeScript(object);
		
		// For pack
		object.setProperty("pack", [](MiniBackgroundObject& owner){}, [](MiniBackgroundObject& owner){return owner.base.getPack();});
		
		object.init();
	}	

/////////////////////////////////////////// EDITOR ///////////////////////////////////////////

#ifdef EDITOR
	
	void MiniBackgroundObject::editor() {
		ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.5-30);
		
////////// Base	//////////
		ImGui::Separator();
		ImGui::Text("Base");
		
		// Pos
		ImGui::DragFloat("##PosX", &pos.x);
		ImGui::SameLine();
		ImGui::DragFloat("##PosY", &pos.y);
		ImGui::SameLine();		
		ImGui::Text("Pos");
		
		// Size
		ImGui::DragFloat("##SizeX", &size.x);
		ImGui::SameLine();
		ImGui::DragFloat("##SizeY", &size.y);
		ImGui::SameLine();		
		ImGui::Text("Size");
		
////////// Visual //////////
		ImGui::Separator();
		ImGui::Text("Visual");
		
		// Color
		ImGui::DragInt("Color", &color, 0, 1);
		
////////// EDITOR THINGS //////////
		ET_propertiesPanel();
		
		ImGui::PopItemWidth();
	}
	
	// 4 states:
	// 0 - unhovered
	// 1 - hovered
	// 2 - selected
	// 3 - dragged
	void MiniBackgroundObject::editor_ObjectFrame(sf::RenderTexture &canvas, int state) {		
		sf::RectangleShape shape;
		shape.setSize({size.x+4, size.y+4});
		shape.setPosition({pos.x-2, pos.y-2});
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