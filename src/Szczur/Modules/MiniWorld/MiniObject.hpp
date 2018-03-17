#pragma once

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Script/Script.hpp"

// #include "Szczur/Modules/MiniWorld/ScriptableBase.hpp"
#include "Szczur/Modules/MiniWorld/Components/Component.hpp"

#ifdef EDITOR
#include "Szczur/Modules/MiniWorld/EditorObject.hpp"
#endif

namespace rat {

struct MiniObject {

	std::string name = "no_name";		
	sf::Vector2f pos{300,200};
	sf::Vector2f size{50,80};
	sf::RectangleShape shape;
	Script &script;
	
	sol::table scriptComponents;
	std::vector<std::unique_ptr<Component>> components;
	
	MiniObject(Script &script);
	
	void update();
	void editor();
	void render(sf::RenderTexture &canvas);
	
	template <typename T>
	void addComponent();
	
	void addComponent(const std::string& componentName);
	
	void runFileScript(const std::string& filepath);		
	void runScript(const std::string& code);	
	static void initScript(Script &script);			
	
	
	
	// virtual void loadScript(const std::string& filepath) = 0;
};

}

#include "MiniObject.tpp"