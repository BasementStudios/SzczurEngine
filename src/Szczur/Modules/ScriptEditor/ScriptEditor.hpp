#pragma once

#include <iostream>

#include <sol.hpp>

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"

#include "ScriptableBase.hpp"
#include "TestObject.hpp"

namespace rat
{

class ScriptEditor : public Module<Window, Input, Script>
{
	std::vector<std::unique_ptr<TestObject>> objects;
	
	sf::RenderTexture canvas;
	
	int selectedObject = -1;
public:

	void addObject() {
		objects.emplace_back(new TestObject);
		objects.back()->setScriptModule(getModule<Script>());	
		objects.back()->getBase().setName(std::string("Object_") + std::to_string(objects.size()));
		// objects.back()->init();
		// objects.back()->loadScript("scripts/button.lua");
	}

	void removeObject(int id) {
		if(id<objects.size() && id>=0) {
			objects.erase(objects.begin()+id);
		}
	}
	
	void initScript() {
		auto module = getModule<Script>().newModule("ScriptEditor");
		getModule<Script>().initClasses<ScriptableBase, TestObject>();
	}
	
	TestObject* lastObject() {
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