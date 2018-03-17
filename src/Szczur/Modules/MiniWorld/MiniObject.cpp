#include "MiniObject.hpp"

#include "Components/ComponentBase.hpp"
#include "Components/ComponentColor.hpp"

namespace rat {

	MiniObject::MiniObject(Script &script) 
		: script(script) {	
		scriptComponents = sol::table::create(script.get().lua_state());
	}

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
		// object.set("pos", &MiniObject::pos);
		// object.set("size", &MiniObject::size);
		// object.set("comp", &MiniObject::scriptComponents);
		object.init();
		
		script.initClasses<ComponentColor, ComponentBase>();
	}
	
/////////////////////////// COMPONENTS ///////////////////////////
	
	void MiniObject::addComponent(const std::string& componentName) {
		//@todo Add checker based on finding
		if(componentName == "Base")  addComponent<ComponentBase>();
		else if(componentName == "Color") addComponent<ComponentColor>();
	}
	
}