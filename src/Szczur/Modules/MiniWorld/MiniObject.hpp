#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Script/Script.hpp"

#include "Szczur/Modules/MiniWorld/MiniFrame.hpp"

namespace rat {

struct MiniObject {

	std::string name = "no_name";		
	sf::Vector2f pos{300,200};
	Script &script;
	MiniFrame frame;
	
	MiniObject(Script &script);
	
/////////////////////////// *ETTERS ///////////////////////////
	void setPosition(const sf::Vector2f& pos);
	const sf::Vector2f& getPosition();
	void setName(const std::string& name);
	const std::string& getName();
	MiniFrame& getFrame();
	
/////////////////////////// MAIN METHODS ///////////////////////////
	virtual void update() = 0;
	virtual void editor() = 0;
	virtual void render(sf::RenderTexture &canvas) = 0;
	
/////////////////////////// SCRIPT ///////////////////////////
	// void runFileScript(const std::string& filepath);		
	// void runScript(const std::string& code);	
	// static void initScript(Script &script);			
};
	
}