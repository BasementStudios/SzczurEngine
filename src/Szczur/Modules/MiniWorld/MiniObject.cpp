#include "MiniObject.hpp"

#include <algorithm>

#include "Components/ComponentBase.hpp"
#include "Components/ComponentColor.hpp"

namespace rat {

	MiniObject::MiniObject(Script &script) 
		: script(script) {	
		scriptComponents = sol::table::create(script.get().lua_state());
	}	
	
/////////////////////////// *ETTERS ///////////////////////////
	
	void MiniObject::setPosition(const sf::Vector2f& _pos) {
		pos = _pos;
	}
	const sf::Vector2f& MiniObject::getPosition(){
		return pos;
	}
	void MiniObject::setSize(const sf::Vector2f& _size) {
		size = _size;
	}
	const sf::Vector2f& MiniObject::getSize(){
		return size;
	}	
	void MiniObject::setName(const std::string& _name) {
		name = _name;
	}	
	const std::string& MiniObject::getName() {
		return name;
	}

/////////////////////////// SEARCHING ///////////////////////////

	Component* MiniObject::findComponent(const std::string& name) {
		auto result = std::find_if(components.begin(), components.end(), [&](auto& obj) { return obj->getComponentName()==name; });
		if(result == components.end()) return nullptr;
		return result->get();
	}

/////////////////////////// MAIN METHODS ///////////////////////////
	
	void MiniObject::update() {
		for(auto& com : components) {
			com->update();
		}
	}

	void MiniObject::editor() {
		for(auto& com : components) {
			com->editor();
		}
	}
	
	void MiniObject::render(sf::RenderTexture &canvas) {
		shape.setSize(size);
		shape.setPosition(pos);
		shape.setOutlineThickness(-3);
		shape.setOutlineColor({0,0,0,255});
		shape.setFillColor({70,70,200,150});
		
		for(auto& com : components) {
			com->render(canvas);
		}
		
		canvas.draw(shape);
	}	
	
/////////////////////////// SCRIPT ///////////////////////////

	void MiniObject::runFileScript(const std::string& filepath) {
		auto& lua = script.get();
		lua.set("THIS", scriptComponents);
		lua.script_file(filepath);
	}
	
	void MiniObject::runScript(const std::string& code) {
		auto& lua = script.get();
		lua.set("THIS", scriptComponents);
		lua.script(code);
	}
	
	void MiniObject::initScript(Script &script) {
		auto object = script.newClass<MiniObject>("MiniObject", "MiniWorld");
		object.init();
		initComponents(script);
	}
	
/////////////////////////// COMPONENTS ///////////////////////////	
	
	void MiniObject::removeComponent(const std::string& componentName) {
		auto result = std::find_if(components.begin(), components.end(), [&](auto& obj) { return obj->getComponentName()==componentName; });
		if(result != components.end()) {
			scriptComponents.set(result->get()->getComponentName(), sol::nil);
			components.erase(result);
		}
	}
	
}