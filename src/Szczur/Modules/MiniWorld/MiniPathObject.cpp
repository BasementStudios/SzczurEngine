#include "MiniPathObject.hpp"

namespace rat {

	void MiniPathObject::renderCollider(sf::RenderTexture &canvas) {
		sf::RectangleShape shape;
		shape.setSize(colliderSize);
		shape.setOutlineThickness(-1);
		shape.setOutlineColor({0,0,200,255});
		shape.setFillColor({0,0,200,80});
		if(centerCollider) { 
			shape.setPosition(pos + colliderRelPos + sf::Vector2f(size.x/2., size.y/2.));
			shape.setOrigin(sf::Vector2f(colliderSize.x/2., colliderSize.y/2.));
		}
		else {
			shape.setPosition(pos + colliderRelPos);
			shape.setOrigin({0,0});
		}
		canvas.draw(shape);		
	}
	
	void MiniPathObject::init() {
		if(funcInit.valid()) funcInit(this);
	}
	
	void MiniPathObject::update() {
		if(funcUpdate.valid()) funcUpdate(this);
	}
	
	void MiniPathObject::render(sf::RenderTexture &canvas) {
		sf::RectangleShape shape;
		shape.setSize(size);
		shape.setPosition(pos);
		shape.setOutlineThickness(-3);
		shape.setOutlineColor({0,0,0,255});
		shape.setFillColor({70,200,70,255});
		canvas.draw(shape);
		
		renderCollider(canvas);
	}
	
	const std::string& MiniPathObject::getName() {
		return name;
	}
	
	void MiniPathObject::setName(const std::string& _name) {
		name = _name;
	}	
	
	const sf::Vector2f& MiniPathObject::getPosition() {
		return pos;
	}	
	void MiniPathObject::setPosition(const sf::Vector2f& _pos) {
		pos = _pos;
	}	
	
/////////////////////////////////////////// SCRIPT ///////////////////////////////////////////
	
	void MiniPathObject::loadScript(const std::string& filepath) {
		base.loadScript<MiniPathObject>(this, filepath);
		init();
	}	
	
	void MiniPathObject::runFileScript(const std::string& filepath) {
		base.runFileScript<MiniPathObject>(this, filepath);
	}
	
	void MiniPathObject::runScript(const std::string& code) {
		base.runScript<MiniPathObject>(this, code);
	}
	
	ScriptableBase& MiniPathObject::getBase() {
		return base;
	}
	
	void MiniPathObject::prepare(Script& script) {
		auto object = base.prepare(script);
		object.set("this", this);
	}
	
	void MiniPathObject::initScript(Script& script) {		
		auto object = script.newClass<MiniPathObject>("MiniPathObject", "MiniWorld");
		
		object.set("pos", &MiniPathObject::pos);
		object.set("size", &MiniPathObject::size);
		object.set("colliderSize", &MiniPathObject::colliderSize);
		object.set("colliderRelPos", &MiniPathObject::colliderRelPos);
		object.set("centerCollider", &MiniPathObject::centerCollider);
		
		object.set("_init", &MiniPathObject::funcInit);
		object.set("_update", &MiniPathObject::funcUpdate);
		object.set("_action", &MiniPathObject::funcAction);
		
		// For pack
		object.setProperty("pack", [](MiniPathObject& owner){}, [](MiniPathObject& owner){return owner.base.getPack();});
		
		object.init();
	}	
	
/////////////////////////////////////////// EDITOR ///////////////////////////////////////////

#ifdef EDITOR
	
	void MiniPathObject::editor() {
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
		
////////// Collider	//////////
		ImGui::Separator();
		ImGui::Text("Collider");
		
		// Pos		
		ImGui::DragFloat("##ColliderPosX", &colliderRelPos.x);
		ImGui::SameLine();
		ImGui::DragFloat("##ColliderPosY", &colliderRelPos.y);
		ImGui::SameLine();		
		ImGui::Text("Rel. pos");
		
		// Size
		ImGui::DragFloat("##ColliderSizeX", &colliderSize.x);
		ImGui::SameLine();
		ImGui::DragFloat("##ColliderSizeY", &colliderSize.y);
		ImGui::SameLine();		
		ImGui::Text("Size");
		
		// Centered
		ImGui::Checkbox("Centered", &centerCollider);
		
////////// EDITOR THINGS //////////
		ET_propertiesPanel();
		
		ImGui::PopItemWidth();
	}
	
	// 4 states:
	// 0 - unhovered
	// 1 - hovered
	// 2 - selected
	// 3 - dragged
	void MiniPathObject::editor_ObjectFrame(sf::RenderTexture &canvas, int state) {		
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