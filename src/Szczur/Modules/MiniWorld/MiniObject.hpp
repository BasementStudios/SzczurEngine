#pragma once

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Script/Script.hpp"

#include "Szczur/Modules/MiniWorld/Components/Component.hpp"

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
	
/////////////////////////// *ETTERS ///////////////////////////
	void setPosition(const sf::Vector2f& pos);
	const sf::Vector2f& getPosition();
	void setSize(const sf::Vector2f& size);
	const sf::Vector2f& getSize();
	void setName(const std::string& name);
	const std::string& getName();
	
/////////////////////////// SEARCHING ///////////////////////////	
	Component* findComponent(const std::string& name);
	
/////////////////////////// MAIN METHODS ///////////////////////////
	void update();
	void editor();
	void render(sf::RenderTexture &canvas);
	
/////////////////////////// SCRIPT ///////////////////////////
	void runFileScript(const std::string& filepath);		
	void runScript(const std::string& code);	
	static void initScript(Script &script);			
	
/////////////////////////// COMPONENTS ///////////////////////////
	template <typename T>
	void addComponent();
	void addComponent(const std::string& componentName);
	void removeComponent(const std::string& componentName); //<--- Body in EditorThings.cpp

	static std::vector<std::string> availableComponents;
	static void initComponents(Script &script);
};
	
}

#include "MiniObject.tpp"