#pragma once

#include <iostream>

#include <sol.hpp>

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"

namespace rat
{

struct ScriptableObject {
	
	sf::Vector2f size{100,100};
	sf::Vector2f pos{200,200};
	int color{0};
	
	std::string name{"no_name"};
	std::string scriptPath = "";
	
	sol::table ref;
	Script *script;
	
	sol::function initFunction;	
	sol::function updateFunction;

	void render(sf::RenderTexture &canvas) {
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
	void setScriptModule(Script& script) {
		this->script = &script;
	}
	
	void setColor(int value) {
		color = value;
	}
	
	void setName(const std::string& name) {
		this->name = name;
	}
	
	void update() {
		if(updateFunction.valid()) updateFunction(this);
	}
	
	void loadScript(const std::string& filepath);
	
	static initScript(Script& script) {
		auto object = script.newClass<ScriptableObject>("ScriptableObject", "ScriptEditor");
		object.set("_init", &ScriptableObject::initFunction);
		object.set("_update", &ScriptableObject::updateFunction);
		object.set("pos", &ScriptableObject::pos);
		object.set("size", &ScriptableObject::size);
		object.set("color", &ScriptableObject::color);
		// object.setProperty("_init", [](){}, [](ScriptableObject &owner, sol::function init){owner.initFunction = init;});
		object.init();
	}		
};

class ScriptEditor : public Module<Window, Input, Script>
{
	std::vector<std::unique_ptr<ScriptableObject>> objects;
	
	sf::RenderTexture canvas;
	
	int selectedObject = -1;
public:

	void addObject() {
		objects.emplace_back(new ScriptableObject);
		objects.back()->setScriptModule(getModule<Script>());	
		objects.back()->setName(std::string("Object_") + std::to_string(objects.size()));
		// objects.back()->loadScript("scripts/button.lua");
	}

	void removeObject(int id) {
		if(id<objects.size() && id>=0) {
			objects.erase(objects.begin()+id);
		}
	}
	
	void initScript() {
		auto module = getModule<Script>().newModule("ScriptEditor");
		getModule<Script>().initClasses<ScriptableObject>();
	}
	
	ScriptableObject* lastObject() {
		return objects.back().get();
	}
	
	void init() {
		// addObject();
		// lastObject()->setName("Object_1");
		// addObject();
		// lastObject()->setName("Object_2");
		// addObject();
		// lastObject()->setName("Object_3");
	}
	
	ScriptEditor()
	{
		LOG_INFO(this, " : Module ScriptEditor constructed");
		auto& window = getModule<Window>().getWindow();
		canvas.create(window.getSize().x, window.getSize().y);
		initScript();
	}
	
	~ScriptEditor()
	{
		LOG_INFO(this, " : Module ScriptEditor destructed");
	}

	void update();
	
	void render();
};

}