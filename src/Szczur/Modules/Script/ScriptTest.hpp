#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include <sol.hpp>

#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/Script/Script.hpp"

#include "Button.hpp"

namespace rat
{

struct ScriptTest : public Module<Canvas, Script, Input>
{
	std::vector<std::unique_ptr<Button>> buttons;
	
	template <typename Tuple>
	ScriptTest(Tuple&& tuple) :
		Module(tuple)
	{
		LOG_CONSTRUCTOR();
		initScript();
		init();
	}
	
	auto addButton(Button* newButton) {		
		buttons.emplace_back(newButton);
		return buttons.back().get();
	}
	
	void reset() {
		buttons.clear();
	}
	
	void render() {
		for(auto& obj : buttons) {
			obj->render(getModule<Canvas>());
		}
	}

	void update() {
		auto& input = getModule<Input>();
		if(input.isPressed(Keyboard::Space)) {
			reset();
			init();
		}
	}	
	
	void init() {
		sol::state& lua = getModule<Script>().get();
		lua.script_file("../src/Szczur/Modules/Script/test.lua");
	}
	
	void initScript() {
		sol::state& lua = getModule<Script>().get();
		auto temp = lua.create_table("ScriptTest");
		
	// Module script
		temp.set_function("addButton", &ScriptTest::addButton, this);
		lua.script_file("../src/Szczur/Modules/Script/ScriptTest.lua");
		
	// Class script
		Button::initScript(getModule<Script>());
	}
};

}

// Overloadings for SFML

// friend sf::Vector2f& operator=(sf::Vector2f& vec, const sol::table table) {
	// vec.x = table[1];
	// vec.y = table[2];
	// return vec;
// }